#pragma once
#include "ShoppingCart.h"

#define CUSTOMER_ID_LENGTH 9
#define NAMES_SEP " "
#define NAME_PARTS_SEP "- "

typedef struct
{
	char*			id;
	char*			name;
	ShoppingCart*	pCart;
}Customer;

int		initCustomer(Customer* pCustomer);
void	getCustomerID(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);

int		isCustomerIdValid(const char* id);

int		isCustomerById(const Customer* pCust, const char* id);
//int		isCustomersNameValid(const char* name);
void	getNamePart(char* part, const char* msg);
void	upperLowerCustomerName(char* name);
//char*	combineFirstLast(char*  firstName, char* lastName);
char*	combineFirstLast(char** parts);
void	pay(Customer* pCustomer);
void	cancelShopping(Customer* pCustomer);

void	freeCustomer(Customer* pCust);



