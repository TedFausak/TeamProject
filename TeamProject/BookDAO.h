#pragma once
#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include "Book.h"

using namespace std;

#ifndef BOOK_DAO_H
#define BOOK_DAO_H

class BookDAO {
private:
	Book books[1024];
	static BookDAO * bookDAO;

	void storeToFile();

public:
	static int numPossibleBooks;
	static int numBooks;

	BookDAO();
	void close();
	static BookDAO * getInstance();
	int getNumBooks() { return numBooks; }

	//CREATE
	void insert(string isbn, string title, string author, string publisher,
		int quantityOnHand, double wholesaleCost, double retailPrice);
	void update(string isbn, string title, string author, string publisher,
		int quantityOnHand, double wholesaleCost, double retailPrice);
	void deleteByIsbn(string isbn);

	//READ
	bool existsByIsbn(string isbn);
	Book * getBooks();
	Book * getBooksByISBN(string keyword);
	Book * getBooksByTitle(string keyword);
	Book * getBooksByAuthor(string keyword);
	Book * getBooksByPublisher(string keyword);
	Book * getBooksByAge(string age);	///
	Book * getBooksByQuantity(int quantity);
	Book * getBooksByWholesaleCost(double wholesaleCost);
	Book * getBooksByRetailPrice(double price);

};

#endif 