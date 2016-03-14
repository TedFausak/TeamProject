#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<time.h>
#include<sstream>
#include<string>

#include "Book.h"
#include "BookDAO.h"
#include "Utils.h"
#include "ReportModule.h"

using namespace std;

BookDAO * BookDAO::bookDAO;
int BookDAO::numBooks = 0;
int BookDAO::numPossibleBooks = 0;

BookDAO::BookDAO() {
	ifstream inputFile("Books.txt");
	string line = "";
	while (getline(inputFile, line)) {
		Book b = Utils::fromCsv(line);
		books[numBooks] = b;
		numBooks++;
	}
	inputFile.close();
}
 
BookDAO * BookDAO::getInstance() {
	if (bookDAO == NULL) {
		bookDAO = new BookDAO();
	}
	return bookDAO;
}

void BookDAO::storeToFile() {
	ofstream outputFile("Books.txt");
	for (int i = 0; i < numBooks; i++) {
		string csvRecord = Utils::toCsv(books[i]);
		outputFile << csvRecord << endl;
	}
	outputFile.close();
}

void BookDAO::close() {
	storeToFile();
}

bool BookDAO::existsByIsbn(string isbn) {
	for (int i = 0; i < numBooks; i++) {
		if (books[i].getIsbn().find(isbn) != std::string::npos)
			return true;
	}
	return false;
}

void BookDAO::insert(string isbn, string title, string author, string publisher,
	int quantityOnHand, double wholesaleCost, double retailPrice) {
	//  Validated  that no book exists by this isbn number.
	if (existsByIsbn(isbn)) {
		cout << "\t\t Book already exist!" << endl;
		return;
	}

	time_t  dateAdded = time(NULL);
	Book b(isbn, title, author, publisher, dateAdded, quantityOnHand, wholesaleCost, retailPrice);
	books[numBooks] = b;
	numBooks++;
	cout << "\t\t Your Book Has Been Inserted Successfully!" << endl;
}

void BookDAO::update(string isbn, string title, string author, string publisher,
	int quantityOnHand, double wholesaleCost, double retailPrice)
{
	Book * b = nullptr;
	int i = 0;
	for (i = 0; i < numBooks; i++)
	{
		if (books[i].getIsbn() == isbn)
		{
			b = &books[i];
			break;
		}
	}
	if (b == nullptr)
	{
		cout << "Book doesn't exist" << endl;
		return;
	}
	b->setTitle(title);
	b->setAuthor(author);
	b->setPublisher(publisher);
	b->setQuantityOnHand(quantityOnHand);
	b->setRetailPrice(retailPrice);
	b->setWholesaleCost(wholesaleCost);
}

void BookDAO::deleteByIsbn(string isbn)
{
	int i = 0;
	for (i = 0; i < numBooks; i++)
	{
		if (books[i].getIsbn() == isbn)
		{
			break;
		}
	}
	if (i == numBooks)
	{
		cout << "Book doesn't exits" << endl;
		return;
	}
	for (int j = i + 1; j < numBooks; j++)
	{
		books[j - 1] = books[j];
	}
	numBooks--;
}

Book * BookDAO::getBooks() {
	Book * copied = new Book[1024];
	for (int i = 0; i < numBooks; i++) {
		copied[i] = books[i];
	}
	return copied;
}

Book * BookDAO::getBooksByISBN(string keyword) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;
	string lowerCaseKeyWord = Utils::toLowerCase(keyword);
	string lowerCaseISBN = "";

	for (int i = 0; i < numBooks; i++) {
		lowerCaseISBN = Utils::toLowerCase(books[i].getIsbn());
		if (lowerCaseISBN.find(lowerCaseKeyWord) != std::string::npos ||
				lowerCaseKeyWord.find(lowerCaseISBN) != std::string::npos)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}

Book * BookDAO::getBooksByTitle(string keyword) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;
	string lowerCaseKeyWord = Utils::toLowerCase(keyword);
	string lowerCaseTitle = "";

	for (int i = 0; i < numBooks; i++) {
		lowerCaseTitle = Utils::toLowerCase(books[i].getTitle());
		if (lowerCaseTitle.find(lowerCaseKeyWord) != std::string::npos ||
				lowerCaseKeyWord.find(lowerCaseTitle) != std::string::npos)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}

Book * BookDAO::getBooksByAuthor(string keyword) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;
	string lowerCaseKeyWord = Utils::toLowerCase(keyword);
	string lowerCaseAuthor = "";

	for (int i = 0; i < numBooks; i++) {
		lowerCaseAuthor = Utils::toLowerCase(books[i].getAuthor());
		if (lowerCaseAuthor.find(lowerCaseKeyWord) != std::string::npos ||
				lowerCaseKeyWord.find(lowerCaseAuthor) != std::string::npos)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}

Book * BookDAO::getBooksByPublisher(string keyword) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;
	string lowerCaseKeyWord = Utils::toLowerCase(keyword);
	string lowerCasePublisher = "";

	for (int i = 0; i < numBooks; i++) {
		lowerCasePublisher = Utils::toLowerCase(books[i].getPublisher());
		if (lowerCasePublisher.find(lowerCaseKeyWord) != std::string::npos ||
				lowerCaseKeyWord.find(lowerCasePublisher) != std::string::npos)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}

Book * BookDAO::getBooksByAge(string age) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;
	time_t date = Utils::fromString(age);

	for (int i = 0; i < numBooks; i++) {
		if (difftime(date, books[i].getDateAdded()) == 0)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	Utils::sortByAge(0, numberPossibleBooks - 1, possibleBooks, ReportModule::listingMode);
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}

Book * BookDAO::getBooksByQuantity(int quantity) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;

	for (int i = 0; i < numBooks; i++) {
		if (books[i].getQuantityOnHand() == quantity)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}

Book * BookDAO::getBooksByWholesaleCost(double wholesaleCost) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;

	for (int i = 0; i < numBooks; i++) {
		if (books[i].getWholesaleCost() == wholesaleCost)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}

Book * BookDAO::getBooksByRetailPrice(double price) {
	Book * possibleBooks = new Book[1024];
	int numberPossibleBooks = 0;

	for (int i = 0; i < numBooks; i++) {
		if (books[i].getRetailPrice() == price)
			possibleBooks[numberPossibleBooks++] = books[i];
	}
	numPossibleBooks = numberPossibleBooks;
	return possibleBooks;
}