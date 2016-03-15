
// Class CashierModule Specification

#include "stdafx.h"
#include <iostream>
#include <string>
#include <stdlib.h>

#include "Module.h"
#include "InventoryModule.h"
#include "Book.h"
#include "BookDAO.h"
#include "Utils.h"

InventoryModule * InventoryModule::inventoryModule;
const int InventoryModule::SHOW_ISBN = 1;
const int InventoryModule::SHOW_TITLE = 2;
const int InventoryModule::SHOW_AUTHOR = 3;
const int InventoryModule::SHOW_PUBLISHER = 4;
const int InventoryModule::SHOW_QUANTITY = 5;
const int InventoryModule::SHOW_DATE = 6;
const int InventoryModule::SHOW_WHOLESALE = 7;
const int InventoryModule::SHOW_RETAIL_PRICE = 8;

InventoryModule::InventoryModule() {
	setName("Inventory Database Module Module");
	setDescription("perform query on a file containing list of books");
}

InventoryModule * InventoryModule::getInstance() {
	if (inventoryModule == NULL) {
		inventoryModule = new InventoryModule();
	}
	return inventoryModule;
}

string InventoryModule::askForISBN() {
	string isbn = "";
	do {
		cout << "\t Enter ISBN:\t";
		cin >> isbn;
		if (isbn.length() != 10 && isbn.length() != 13)
			cout << "Invalid ISBN! There Has To Be Exactly 10  or 13 Digits!" << endl;
	} while (isbn.length() != 10 && isbn.length() != 13);
	return isbn;
}

int InventoryModule::showChoices(int lowerBound, int upperBound) {
	int choice = 0;
	do {
		cout << "\t\t Enter Your Choice: ";
		cin >> choice;
		if (choice < lowerBound || choice > upperBound)
			cout << "\t\t Invalid Option. Please Enter Your Choice Again!!!" << endl << endl;
	} while (choice < lowerBound || choice > upperBound);
	return choice;
}

void InventoryModule::display() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t     Inventory Database" << endl << endl;
	cout << "\t\t 1. Look Up a Book" << endl;
	cout << "\t\t 2. Add a Book" << endl;
	cout << "\t\t 3. Edit a Book's Record" << endl;
	cout << "\t\t 4. Delete a Book" << endl;
	cout << "\t\t 5. Return to the Main Menu" << endl << endl;

	switch (showChoices(1,5)) {
	case 1:
		displayLookUpMenu();
		break;
	case 2:
		displayAdd();
		break;
	case 3:
		displayEdit();
		break;
	case 4:
		displayDelete();
		break;
	case 5:
		Module::showMainMenu();
	}
	display();
}

void InventoryModule::displayLookUpMenu() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up A Book" << endl << endl;
	cout << "\t\t 1. Look Up By ISBN" << endl;
	cout << "\t\t 2. Look Up By Title" << endl;
	cout << "\t\t 3. Look Up By Author" << endl;
	cout << "\t\t 4. Look Up By Publisher" << endl;
	cout << "\t\t 5. Look Up By Age (Date Added)" << endl;
	cout << "\t\t 6. Look Up By Quantity On Hand" << endl;
	cout << "\t\t 7. Look Up By Wholesale Cost" << endl;
	cout << "\t\t 8. Look Up By Retail Price" << endl;
	cout << "\t\t 9. Return to the Inventory Module" << endl << endl;

	switch (showChoices(1, 9)) {
	case 1:
		showBooksByISBN();
		break;
	case 2:
		showBooksByTitle();
		break;
	case 3:
		showBooksByAuthor();
		break;
	case 4:
		showBooksByPublisher();
		break;
	case 5:
		showBooksByDate();
		break;
	case 6:
		showBooksByQuantity();
		break;
	case 7:
		showBooksByWholesale();
		break;
	case 8:
		showBooksByRetailPrice();
		break;
	}
}

void InventoryModule::displayOptionsAfterLookUp(int thingToShow, Book bookObtained) {
	cout << "\t\t 1. Look Up Another Book" << endl;
	cout << "\t\t 2. Back To Look Up Menu" << endl;
	cout << "\t\t 3. Add This Book To Cart" << endl << endl;

	switch (showChoices(1, 3)) {
	case 1:

		switch (thingToShow) {
		case SHOW_ISBN:
			showBooksByISBN();
			break;
		case SHOW_TITLE:
			showBooksByTitle();
			break;
		case SHOW_AUTHOR:
			showBooksByAuthor();
			break;
		case SHOW_PUBLISHER:
			showBooksByPublisher();
			break;
		case SHOW_DATE:
			showBooksByDate();
			break;
		case SHOW_QUANTITY:
			showBooksByQuantity();
			break;
		case SHOW_WHOLESALE:
			showBooksByWholesale();
			break;
		case SHOW_RETAIL_PRICE:
			showBooksByRetailPrice();
			break;
		}

		break;
	case 2:
		displayLookUpMenu();
		break;
	case 3:
		int howMany = -1;
		do {
			cin >> howMany;
			if (howMany < 0)
				cout << "Number Of Books Has To Be An Integer Greater Than 0. Please Enter Again" << endl << endl;
			if (howMany > bookObtained.getQuantityOnHand())
				cout << "We Only Have " << bookObtained.getQuantityOnHand() << " In Stock. Please Enter Again" << endl << endl;
		} while (howMany < 0 || howMany > bookObtained.getQuantityOnHand());
		cout << "How Many Books Do You Want? ";
		

	
		break;

	}
}


void InventoryModule::displayAdd() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Add A Book" << endl << endl;

	string isbn = askForISBN(), 
		   title = "", author = "", publisher = "";
	int quantity = 0;
	double wholesaleCost = 0, retailPrice = 0;

	cin.ignore();
	cout << "\t Enter Title:             ";
	getline(cin, title);
	cout << "\t Enter Author:            ";
	getline(cin, author);
	cout << "\t Enter Publisher:         ";
	getline(cin, publisher);
	cout << "\t Enter Quantity-On-Hand:  ";
	cin >> quantity;
	cout << "\t Enter Wholesale Cost     ";
	cin >> wholesaleCost;
	cout << "\t Enter Retail Price       ";
	cin >> retailPrice;

	cout << "\n\t\t Do You Want To Add This Book?" << endl;
	cout << "\t\t 1. Yes, I Want To Add This Book" << endl;
	cout << "\t\t 2. No, I Want To Go Back To Inventory Menu" << endl << endl;

	if (showChoices(1,2) == 1) {
		BookDAO::getInstance()->insert(isbn, title, author,
			publisher, quantity, wholesaleCost, retailPrice);
		system("pause");
	}
}

void InventoryModule::displayEdit() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Edit A Book's Record" << endl << endl;
	
	string isbn = askForISBN();

	Book * book = BookDAO::getInstance()->getBooksByISBN(isbn);
	if (BookDAO::numPossibleBooks == 0) 
		cout << "Sorry! Your ISBN Cannot Be Found";
	else {
		cout << endl << "\t\t Book Information" << endl;
		Utils::displayBookInformation(book[0]);
		if (displayEditOptions(isbn) == false)
			return;
		cout << "Your Book Has Been Edited Successfully!" << endl << endl;
	}

	cout << "\t\t 1. Edit Another Book" << endl;
	cout << "\t\t 2. Back To Look Up Menu" << endl << endl;

	if (showChoices(1, 2) == 1)
		displayEdit();
}

bool InventoryModule::displayEditOptions(string isbn) {
	string title = "", author = "", publisher = "";
	int quantity = 0;
	double wholesaleCost = 0, retailPrice = 0;

	cin.ignore();
	cout << "\t Enter Title:             ";
	getline(cin, title);
	cout << "\t Enter Author:            ";
	getline(cin, author);
	cout << "\t Enter Publisher:         ";
	getline(cin, publisher);
	cout << "\t Enter Quantity-On-Hand:  ";
	cin >> quantity;
	cout << "\t Enter Wholesale Cost:    ";
	cin >> wholesaleCost;
	cout << "\t Enter Retail Price:      ";
	cin >> retailPrice;

	cout << "\n\t\t Do You Want To Edit This Book?" << endl;
	cout << "\t\t 1. Yes, I Want To Edit This Book" << endl;
	cout << "\t\t 2. No, I Want To Go Back To Inventory Menu" << endl << endl;

	if (showChoices(1,2) == 1) {
		BookDAO::getInstance()->update(isbn, title, author,
			publisher, quantity, wholesaleCost, retailPrice);
		return true;
	}
	return false;
}

void InventoryModule::displayDelete() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Delete A Book" << endl << endl;
	cout << "\t\t Enter ISBN: ";
	string isbn = "";
	cin >> isbn;
	BookDAO::getInstance()->deleteByIsbn(isbn);
	system("pause");
}

void InventoryModule::showBooksByISBN() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By ISBN" << endl << endl;
	cout << "\t\t     Enter ISBN: ";
	string keyword = "";
	cin.ignore();
	getline(cin, keyword);
	Book * possibleBooks = BookDAO::getInstance()->getBooksByISBN(keyword);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:\t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:\t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:\t" << possibleBooks[i].getAuthor() << endl << endl;
		}
	
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
	
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_ISBN, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your ISBN Cannot Be Found!" << endl;
		system("pause");
	}
}

void InventoryModule::showBooksByTitle() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By Title" << endl << endl;
	cout << "\t\t     Enter Title: ";
	string keyword = "";
	cin.ignore();
	getline(cin, keyword);
	Book * possibleBooks = BookDAO::getInstance()->getBooksByTitle(keyword);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:\t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:\t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:\t" << possibleBooks[i].getAuthor() << endl << endl;
		}
	
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
		
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_TITLE, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your Title Cannot Be Found!" << endl;
		system("pause");
	}
}

void InventoryModule::showBooksByAuthor() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By Author" << endl << endl;
	cout << "\t\t     Enter Author: ";
	string keyword = "";
	cin.ignore();
	getline(cin, keyword);
	Book * possibleBooks = BookDAO::getInstance()->getBooksByAuthor(keyword);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:\t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:\t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:\t" << possibleBooks[i].getAuthor() << endl;
		}
	
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
		
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_AUTHOR, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your Author Cannot Be Found!" << endl;
		system("pause");
	}
}

void InventoryModule::showBooksByPublisher() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By Publisher" << endl << endl;
	cout << "\t\t     Enter Publisher: ";
	string keyword = "";
	cin.ignore();
	getline(cin, keyword);
	Book * possibleBooks = BookDAO::getInstance()->getBooksByPublisher(keyword);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:     \t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:    \t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:   \t" << possibleBooks[i].getAuthor() << endl;
			cout << "Publisher:\t" << possibleBooks[i].getPublisher() << endl;
		}
		
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
	
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_PUBLISHER, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your Publisher Cannot Be Found!" << endl;
		system("pause");
	}
}

void InventoryModule::showBooksByDate() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By Date Added" << endl << endl;
	cout << "\t\t   Enter Date Added: ";
	string dateAdded = "";
	cin.ignore();
	getline(cin, dateAdded);

	Book * possibleBooks = BookDAO::getInstance()->getBooksByAge(dateAdded);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:\t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:\t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:\t" << possibleBooks[i].getAuthor() << endl;
		}
	
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
		
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_DATE, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your Date Added Cannot Be Found!" << endl;
		system("pause");
	}
}

void InventoryModule::showBooksByQuantity() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By Quantity" << endl << endl;
	cout << "\t\t     Enter Quantity: ";
	int quantity = 0;
	cin >> quantity;
	Book * possibleBooks = BookDAO::getInstance()->getBooksByQuantity(quantity);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:\t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:\t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:\t" << possibleBooks[i].getAuthor() << endl;
		}
		
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
		
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_QUANTITY, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your Quantity Cannot Be Found!" << endl;
		system("pause");
	}
}

void InventoryModule::showBooksByWholesale() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By Wholesale Cost" << endl << endl;
	cout << "\t\t   Enter Wholesale Cost: ";
	double wholesaleCost = 0;
	cin >> wholesaleCost;
	Book * possibleBooks = BookDAO::getInstance()->getBooksByWholesaleCost(wholesaleCost);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:\t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:\t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:\t" << possibleBooks[i].getAuthor() << endl;
		}
	
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
	
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_WHOLESALE, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your Wholesale Cost Cannot Be Found!" << endl;
		system("pause");
	}
}

void InventoryModule::showBooksByRetailPrice() {
	system("CLS");
	cout << "\t\t  Serendipity Booksellers" << endl;
	cout << "\t\t    Look Up By Retail Price" << endl << endl;
	cout << "\t\t   Enter Retail Price: ";
	double retailPrice = 0;
	cin >> retailPrice;
	Book * possibleBooks = BookDAO::getInstance()->getBooksByRetailPrice(retailPrice);

	if (BookDAO::numPossibleBooks != 0) {
		for (int i = 0; i < BookDAO::numPossibleBooks; i++) {
			cout << i + 1 << endl;
			cout << "ISBN:\t" << possibleBooks[i].getIsbn() << endl;
			cout << "Title:\t" << possibleBooks[i].getTitle() << endl;
			cout << "Author:\t" << possibleBooks[i].getAuthor() << endl;
		}
		
		int choice = showChoices(1, BookDAO::numPossibleBooks) - 1;
		system("CLS");
		cout << "\t\t  Serendipity Booksellers" << endl;
		cout << "\t\t      Book Information" << endl << endl;
	
		Utils::displayBookInformation(possibleBooks[choice]);
		displayOptionsAfterLookUp(SHOW_RETAIL_PRICE, possibleBooks[choice]);
	}
	else {
		cout << "\t\t Sorry! Your Retail Price Cannot Be Found!" << endl;
		system("pause");
	}
}