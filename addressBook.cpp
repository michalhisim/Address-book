// addressBook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

#include <iomanip> // cout output formating
#include <cstdlib> // clearing, pausing console
#include <vector>  // dynamicly alocated resizable array
#include <conio.h> // for advanced menu
#include <fstream> // Files
#include <sstream> // string to int


using namespace std;

//! Helper class
class Project
{

public:
	string name;
	string author;
	string version;

	//! Constructor
	Project()
	{
		this->name = "Address Book";
		this->author = "Michal Simon (SIM0119)";
		this->version = "0.1 (Alfa)";
	}
};

//! Contact class
class Contact
{
public:
	
	int phone;
	string name;
	string surname;

	//! constructor - create an anonymouse person
	Contact()
	{
		this->name    = "Anonymous";
		this->surname = "Anonym";
		this->phone   = 123456789;
	}

	//! converts string to integer
	/*!
	@param phone
	@return success/error
	*/
	bool addPhone(string phone)
	{
		istringstream is(phone);
		int number;
		if((is >> number).fail())
		{ 
			// Error
			return false;
		} 
		
		this->phone = number;
		return true;
	}

	//! clean the contact
	void destroy()
	{
		Contact::~Contact();
	}

};

//! Group of contacts
class AddressBook
{
	// config
	int rowWidth;

public:
	vector<Contact> book;

	//! constructor
	AddressBook()
	{
		//Config

		this->rowWidth = 20;
	}

	//! Add a new contacts
	/*!
	@param person - a new contact
	@return id of added contact
	*/
	int addContact(Contact person)
	{
		this->book.push_back(person);
	
		return this->book.size()-1;
	}

	//! Getter of contact
	/*!
	@param id - index of contact
	@return Contact
	*/
	Contact getContact(int id)
	{
		if(id>=0 && id<this->book.size())
		{		
			return this->book[id];
		}
		else
		{
			Contact person;

			return person;
		}
	}

	//! Delete the contact from address book
	/*!
	@param id - index of contact
	@return - success/error
	*/
	bool deleteContact(int id)
	{
		if(id>=0 && id<this->book.size())
		{
			this->book.erase(this->book.begin()+id);

			return true;
		}
		else
		{
			return false;
		}
	}

	//! Print all contacts
	void print()
	{
		int size = this->book.size();

		Contact person;

		cout << endl;

		cout << right << setw(5) << "#" << "  "
			 << left << setw(this->rowWidth) << "Name" << " "
			 << left << setw(this->rowWidth) << "Surname"
			 << left << setw(10) << "Phone" << endl;
		cout << " __________________________________________________________" << endl << endl;

		if(size>0)
		{
			for(int i=0;i<size;i++)
			{
				person = this->book.at(i);
			
				cout << right << setw(5) << i << ". "
					 << left << setw(this->rowWidth) << person.name << " "
					 << left << setw(this->rowWidth) << person.surname
					 << left << setw(10) << person.phone << endl;
			}
		}
		else
		{
			cout << "                      No contacts" << endl;
		}

		cout << " __________________________________________________________" << endl << endl;

	}

	//! Import contacts from CSV file
	/*!
	@param path - file name and path
	@return - success/error
	*/
	bool importFromCsv(string path)
	{
		ifstream file;

		file.open(path);

		if(file.is_open())
		{
			
			string value;

			int size = 0;

			Contact person, anonym;
			bool quote = false;
			int parameter = 1;

			string parameter_val = "";
			string previous;

			while(file.good())
			{
				value = file.get();

				if(value=="\"")
				{
					quote = !quote;
				}
				else if(value==",")
				{
					if(!quote)
					{
						// Save

						if(parameter==1)
						{
							person.name = parameter_val;
						}
						else if(parameter==2)
						{
							person.surname = parameter_val;
						}
						else if(parameter==3)
						{
							person.addPhone(parameter_val);
						}

						parameter_val = "";

						parameter++;
					}
				}
				else if(value=="\n")
				{
					this->addContact(person);

					// Recreate person
					person = anonym;

					parameter = 1;
					parameter_val = "";
				}
				else
				{
					parameter_val.append(value); 
				}

				previous = value;

				//cout << value << " - " << quote << " " << parameter_val << " - " << parameter << endl;
			}
			
			file.close();
			
			return true;
		}
		else
		{
			return false;
		}
	}

	//! Export contacts to CSV
	/*!
	@param path - file name and path
	@return - success/error
	*/
	bool export2Csv(string path)
	{
		ofstream file;

		file.open(path);

		if(file.is_open())
		{			
			int size = this->book.size();

			for(int i = 0;i<size;i++)
			{
				file << "\"" << this->book[i].name << "\",\"" << this->book[i].surname << "\",\"" << this->book[i].phone << "\"\n";
			}

			
			file.close();
			return true;
		}
		else
		{
			return false;
		}
	}


	//! Export contacts to HTML
	/*!
	@param path - file name and path
	@return - success/error
	*/
	bool export2Html(string path)
	{
		
		string html = "";
		string output = "";
		string title = "Address Book";

		// header

		output.append("<!doctype html>\n");
		output.append("<html>\n");
		output.append("<head>\n");
		output.append("<meta charset=\"utf-8\">\n");
		output.append("<title>");
		output.append(title);
		output.append("</title>\n");
		output.append("<style>\n");
		output.append("body{font:13px/1.231 sans-serif; *font-size:small;text-align:center}\n");
		output.append("#page{text-align:left;width:950px;margin:auto}\n");
		output.append("h1{text-align:center}\n");
		output.append("table{width:100%;border: 1px solid #c44206;background:#c44206;} th{color:#fff} td{background: #FBFBFB}\n");
		output.append("tr:nth-child(even) td{background: #CDCDCD}\n");
		output.append("#footer{margin:18px}</style>\n");
		output.append("</head>\n");
		output.append("<body>\n");
		output.append("<div id=\"page\">\n");
		output.append("<h1>");
		output.append(title);
		output.append("</h1>");
		output.append("<table cellspacing=\"1\" cellpadding=\"5\">\n");
		output.append("<tr><th>Name</th><th>Surname</th><th>Phone</th></tr>\n");

		int size = this->book.size();
		char buffer [33];

		for(int i=0;i<size;i++)
		{
			html.append("<tr><td>");
			html.append(this->book[i].name);
			html.append("</td><td>");
			html.append(this->book[i].surname);
			html.append("</td><td>");
			html.append(itoa(this->book[i].phone,buffer,10)); // int to string
			html.append("</td></tr>\n");

			output.append(html);
		}

		// Footer
		output.append("</table></div>\n");
		output.append("<div id=\"footer\">Copyright 2011 <a href=\"http://hisim.cz\" target=\"blank\"><img src=\"http://hisim.cz/images/hisim_logo_copyright.png\" border=\"0\" alt=\"HISIM Studio\" /></a>. All rights reserved.</div>\n");
		output.append("</body></html>");

		ofstream file;

		file.open(path);

		if(file.is_open())
		{			
			file << output;
			
			file.close();
			return true;
		}
		else
		{
			return false;
		}
	}
};

//! Menu class
class Menu
{
	vector<string> menu;

	string title;

	Project project;

	int width;

public:

	int selectedIndex;
	bool running;
	int menuItems;

	//! constructor
	/*!
	@param project - info about the application
	*/
	Menu(Project project)
	{
		this->project = project;
		
		this->selectedIndex = 0;
		this->running = true;
		this->menuItems = 0;

		this->width = 30;
	}

	//! Add a menu item to the menu list
	/*!
	@param item - name of new menu item
	@return bool - success/error
	*/
	bool addItem(string item)
	{
		this->menu.push_back(item);

		this->menuItems = this->menu.size();

		return true;
	}

	//! Add a menu title
	/*!
	@param title - name of menu
	@return bool - success/error
	*/
	bool addTitle(string title)
	{
		this->title = title;
	}

	//! Filters the key presses
	/*!
	@param key - code of key
	@return bool - moove or not
	*/
	bool extendedKeyPress(int key)
	{
		bool move = false;

		switch (key)
		{
			case 72: // Up arrow
				
				if (this->selectedIndex > 0)
				{
					this->selectedIndex--;
					move = true;
				}
				break;

			case 80: // Down arrow
				if (this->selectedIndex < (this->menuItems-1))
				{
					this->selectedIndex++;
					move = true;
				}
				break;
		}
		return move;
	}

	//! print the menu on the screen
	void drawMenu()
	{
		int item;
		
		system("cls");

		cout << " -------------------------------------------------" << endl
			 << " Project:    " << this->project.name << endl
			 << " Author:     " << this->project.author << endl
			 << " Version:    " << this->project.version << endl
			 << " -------------------------------------------------" << endl << endl;

		for (item = 0; item < this->menuItems; item++)
		{			
			if (item == this->selectedIndex)
			{
				// Drawing the selected item

				cout << " --> " << left << setw(this->width) << this->menu[item] << " <-- " << endl;
			}
			else
			{
				// Drawing the un-selected item

				cout << "     " << left << setw(this->width) << this->menu[item] << endl;
			}

			//cout << this->menu[item] << endl;
		}


	}

};



int _tmain(int argc, _TCHAR* argv[])
{
	// Config
	
	Project project;
	
	AddressBook book;

	project.version = "0.5";

	Menu menu(project);

	menu.addItem("Add contact");
	menu.addItem("Edit contact");
	menu.addItem("Delete contact");
	menu.addItem("View all contacts");
	menu.addItem("Delete all contacts");
	menu.addItem("Save all contacts to file");
	menu.addItem("Read contacts from file");
	menu.addItem("Export all contacts to HTML");
	menu.addItem("Exit");

	int key;
	bool refreshMenu = true;
	string title;
	string letter;
	string line = " -------------------------------------------------------";

	Contact person;

	// -------------------------------------------

	system("Color 4E");

	while (menu.running)
	{

		// Redraw menu only if selection realy change
		if (refreshMenu)
		{
			menu.drawMenu();
		}

		key = getch();

		if(key==13)
		{
			system("cls");

			// Enter key
			switch(menu.selectedIndex)
			{
			case 0:
				{
					// Add contact
						
					title = " Adding a new person. Follow theese steps, please. ";

					cout << title << "(1/3)" << endl << line << endl; 

					cout << " First name: ";

					cin >> person.name;

					system("cls");

					cout << title << "(2/3)" << endl << line << endl;

					cout << " Surname: ";

					cin >> person.surname;

					system("cls");

					cout << title << "(3/3)" << endl << line << endl;

					string phone;

					cout << " Phone number: ";

					cin >> phone;

					person.addPhone(phone);

					book.addContact(person);

					break;
				}

			case 1:
				{
					// Edit contact
					
					if(book.book.size()>0)
					{
						book.print();
						
						cout << " Which contact do you want to edit?" << endl << " Choose one of the contacts above (index): ";

						int index;

						cin >> index;

						system("cls");

						if(index>=0 && index<book.book.size())
						{
							person = book.getContact(index);
							
							cout << " Person: " << person.name << " " << person.surname << " - " << person.phone << endl << line << endl << " What would you like to edit? [(N)ame, (S)urname, (P)hone]: ";

							cin >> letter;

							cout << endl;

							if(letter=="N" || letter=="n")
							{
								cout << " Enter a new name for this contact: ";

								cin >> book.book[index].name;

							}
							else if(letter=="S" || letter=="s")
							{
								cout << " Enter a new surname for this contact: ";

								cin >> book.book[index].surname;

							}
							else if(letter=="P" || letter=="p")
							{
								cout << " Enter a new phone for this contact: ";

								cin >> book.book[index].phone;

							}
							else
							{
								cout << endl << " This was wrong choice. Sorry!" << endl << endl;
								
								system("pause");
							}
						}
						else
						{							
							cout << endl << " This contact does not exists. Sorry! " << endl << endl;

							system("pause");
						}
					}
					else
					{
						cout << endl << " There are no contacts. Sorry! " << endl << line << endl;

						system("pause");
					}

					break;
				}

			case 2:
				{
					//Delete contact

					if(book.book.size()>0)
					{
						book.print();
						
						cout << " Which contact do you want to delete?" << endl << " Choose one of the contacts above (index): ";

						int index;

						cin >> index;

						system("cls");	

						if(index>=0 && index<book.book.size())
						{
							person = book.getContact(index);
							
							cout << " Delete person: " << person.name << " " << person.surname << " - " << person.phone << endl << line << endl << " Are you sure? (y/n): ";

							cin >> letter;

							cout << endl;

							if(letter=="Y" || letter=="y")
							{
								book.deleteContact(index);
							}
						}
						else
						{
							cout << endl << " This contact does not exists. Sorry! " << endl << endl;

							system("pause");
						}

					}
					else
					{
						cout << endl << " There are no contacts. Sorry! " << endl << line << endl;

						system("pause");						
					}

					break;
				}

			case 3:
				{
					//View all contact

					book.print();
				
					system("pause");

					break;
				}

			case 4:
				{
					// Delete all contacts

					cout << endl << " Are you sure you want to delete all contacts? (y/n): ";

					cin >> letter;

					if(letter == "Y" || letter == "y")
					{
						book.book.clear();
					}
					else
					{
						refreshMenu = true;
					}

					break;
				}

			case 5:
				{
					// Save all contacts

					string filename;

					cout << endl << " Save all contacts to csv file!" << endl << line << endl << " Choose a filename (contacts.csv): ";

					cin >> filename;

					cout << endl;

					if(book.export2Csv(filename))
					{
						cout << " Success!" << endl;
					}
					else
					{
						cout << " Fail! " << endl;
					}

					cout << endl;

					system("pause");

					break;
				}

			case 6:
				{
					// Read contacts from file

					string filename;

					cout << endl << " Read all contacts from csv file!" << endl << line << endl << " Choose a filename (contacts.csv): ";

					cin >> filename;

					cout << endl;

					if(book.importFromCsv("pokus.csv"))
					{
						cout << " Success!" << endl;
					}
					else
					{
						cout << " Fail! " << endl;
					}

					cout << endl;

					system("pause");

					break;
				}

			case 7:
				{
					// Save all contacts to HTML

					string filename;

					cout << endl << "Export all contacts to HTML file!" << endl << line << endl << " Coose a filename (contacts.html): ";

					cin >> filename;

					cout << endl;

					if(book.export2Html(filename))
					{
						cout << " Success!" << endl;
					}
					else
					{
						cout << " Fail! " << endl;
					}

					cout << endl;

					system("pause");

					break;
				}

			default:
				{
					// Exit
				
					cout << endl << " Copyright 2011 Michal Simon (http://hisim.cz)" << endl << line << endl;
					
					cout << endl << " Are you sure you want to exit? (y/n): ";

					cin >> letter;

					if(letter == "Y" || letter == "y")
					{
						return 0;
					}
					else
					{
						refreshMenu = true;
					}
				}
			}		
		}
		else
		{
			refreshMenu = menu.extendedKeyPress(key);
		}
	}

	system("cls");

	return 0;
}