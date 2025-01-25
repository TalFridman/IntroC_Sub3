#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"

#define BIN_FILE "SuperMarket.bin"
#define TEXT_FILE "Customers.txt"

int main()
{
	srand((unsigned int)time(NULL));

	SuperMarket* pMarket = readAllSuperMarketFromBinaryFile(BIN_FILE);
	SuperMarket market;

	if (!pMarket)
	{
		printf("Error open super market file!\n");                                     
		if (!initSuperMarket(&market))
		{
			printf("error init Super Market\n");
			return 0;
		}
	}
	else
	{
		market = *pMarket;
		market.customerArr = readAllCustomersFromTxtFile(TEXT_FILE, &market.customerCount);
		if (!market.customerArr)
			printf("Error open customer file!\n");
		else
			printf("SuperMarket succsesfully loaded from files!\n");
	}

	int option;
	int stop = 0;
	
	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;

		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if(!doShopping(&market))
				printf("Error in shopping\n");
			break;

		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerManageShoppingCart:
			if(!manageShoppingCart(&market))
				printf("Error in shopping cart managment\n");
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;
		
		case eSortProducts:
			sortProductsByAtt(&market);
			break;

		case eSearchProductByAttribute:
			searchProductByAtt(&market);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	if(!writeAllCustomersToTextFile(TEXT_FILE, &market))
		printf("Error in write all customers to text file!\n");

	if (!writeAllSuperMarketToBinaryFile(BIN_FILE, &market))
		printf("Error in write all supermarket to binary file!\n");
	
	freeMarket(&market);
	
	return 1;
}

int menu()
{
	int option;
	printf("\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}
