#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "General.h"
#include "ShoppingCart.h"
#include "ClubMember.h"

static const char* sortOptions[eNumOfSorts] = { "Sort By Name", "Sort By Count", "Sort By Price" };



int		initSuperMarket(SuperMarket* pMarket)
{
	pMarket->productArrSortBy = eNumOfSorts;       // Default - Non Sorted!
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->productCount = 0;
	pMarket->productArr = NULL;
	pMarket->name = getStrExactLength("Enter market name");

	if (!pMarket->name)
		return 0;

	return 1;
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char answer;
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = NULL;

	printf("\n");
	printf("Adding new product? y/Y: ");
	scanf("%c", &answer);
	getchar();

	if (toupper(answer) == 'Y')
	{
		if (!addNewProduct(pMarket))
		{
			free(pProd);
			return 0;
		}
		return 1;
	}
	else if (pMarket->productCount > 0)
	{
		printf("Do you want to increase the amount of an existing product? y/Y: ");
		scanf("%c", &answer);
		getchar();
		if (toupper(answer) == 'Y')
		{
			printAllProducts(pMarket);
			pProd = getProductFromUser(pMarket, barcode);
			if (pProd != NULL) //This barcode exist in stock
				updateProductCount(pProd);
		}
	}
	else
		return 0;
	
	return 1;
}

int		addNewProduct(SuperMarket* pMarket)
{

	Product** pTempArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	if (!pTempArr)
	{
		freeProducts(pMarket);
		return 0;
	}
	pMarket->productArr = pTempArr;
	
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(pMarket->productArr);
		return 0;
	}

	initProductNoBarcode(pProd);

	do
	{
		generateBarcode(pProd);
	} while (!isBarcodeUnique(pMarket, pProd->barcode)); //generate new barcode until it is unique
	
	pMarket->productArr[pMarket->productCount] = pProd;
	pMarket->productCount++;
	return 1;
}

int		isBarcodeUnique(const SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (strcmp(pMarket->productArr[i]->barcode, barcode) == 0)
			return 0; // Barcode is not unique
	}
	return 1; // Barcode is unique
}

int isCustomerIdUnique(const SuperMarket* pMarket, const char* id)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].id, id) == 0)
		{
			printf("ID %s is not unique\n", id);
			return 0; // ID is not unique
		}
	}
	return 1; // ID is unique
}

int		addCustomer(SuperMarket* pMarket)                                      
{
	Customer cust = { 0 };
	int choice = -1;
	int notFirstLoop = 0;

	printf("Is the customer club member? 1 for yes 0 for no\n");
	do {
		scanf("%d", &choice);
		if (choice != 0 && choice != 1)
			printf("Invalid input! please enter 1 for yes 0 for no\n");
	} while (choice != 0 && choice != 1);

	do {
		if (notFirstLoop)
			cust.vTable.freeObject(&cust);

		if (choice == 0)
		{
			if (!initCustomer(&cust))
			{
				freeCustomer(&cust);
				return 0;
			}
		}
		else
		{
			if (!initClubMember(&cust))                                           
			{
				freeClubMember(&cust);
				return 0;
			}
		}
		notFirstLoop++;
	} while (!isCustomerIdUnique(pMarket, cust.id));

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)safeRealloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;
	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);                                            
	NODE* temp = pCustomer->pCart->shoppingItemList.head.next;
	if(!(temp))                                                                    //did not buy any thing
	{
		freeShoppingCart(pCustomer->pCart);                                              
		pCustomer->pCart->shoppingItemList.head.next = NULL;
	}
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)                            
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart->shoppingItemList.head.next == NULL)                                               
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	float totalPrice = printShoppingCart(pCustomer->pCart);
	if (!pCustomer->pDerived)
		printf("for %s is %.2f\n", pCustomer->name, totalPrice);
	else
	{
		ClubMember* pClubMember = (ClubMember*)pCustomer->pDerived;
		float precentDis = calculatePrice(pClubMember, &totalPrice); 
		printf("for %s is %.2f, after discount of %.2f %%\n", pCustomer ->name , totalPrice, precentDis);
	}
	return pCustomer;
}

int	manageShoppingCart(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	char answer;

	if(!pCustomer)
		return 0;

	printf("Do you want to pay for the cart? y/Y, anything else to cancel shopping!\t");
	do {
		scanf("%c", &answer);
	} while (isspace(answer));

	getchar(); //clean the enter

	if (answer == 'y' || answer == 'Y')
		//pay(pCustomer);
		pCustomer->vTable.pay(pCustomer);
	else {
		clearCart(pMarket, pCustomer);
		cancelShopping(pCustomer);
	}
	return 1;

}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer is not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->productCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %-20s %-15s\n", "Type", "Price", "Count In Stoke", "Expiry Date");
	printf("-------------------------------------------------------------------------------------------------\n");

	generalArrayFunction(pMarket->productArr, pMarket->productCount, sizeof(Product*), printProduct);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
		pMarket->customerArr[i].vTable.print(&pMarket->customerArr[i]);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char searchTerm[MAX_STR_LEN];
	getsStrFixSize(searchTerm, sizeof(searchTerm), "Who is shopping? Enter customer id\n");

	Customer* pCustomer = FindCustomerById(pMarket, searchTerm);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket, ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

void clearCart(SuperMarket* pMarket, Customer* pCustomer)
{

	NODE* temp = pCustomer->pCart->shoppingItemList.head.next;
	if (!temp)
		return;
	
	while (temp)
	{
		ShoppingItem* tempShoppingItem = temp->key;
		Product* pProd = getProductByBarcode(pMarket, tempShoppingItem->barcode);
		if (pProd)
			pProd->count += tempShoppingItem->count;
		temp = temp->next;
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product is out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (pMarket->productArr[i]->type == type)
		{
			count++;
			printProduct(pMarket->productArr[i]);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBarcodeCode(barcode);
	Product* pProd = getProductByBarcode(pMarket, barcode);
	if (!pProd)
	{
		printf("No such product barcode\n");
		return NULL;
	}
	return pProd;
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeProducts(pMarket);
	freeCustomers(pMarket->customerArr, pMarket->customerCount);
}

void freeProducts(SuperMarket* pMarket)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		freeProduct(pMarket->productArr[i]);
		free(pMarket->productArr[i]);
	}
	free(pMarket->productArr);
}

void freeCustomers(Customer* pCustomersArr, int numOfCustomers)
{
	for (int i = 0; i < numOfCustomers ; i++)
		pCustomersArr[i].vTable.freeObject(&pCustomersArr[i]);
	free(pCustomersArr);
}

void sortProductsByAtt(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("\nNo Products yet!\n");
		return;
	}
	eSort option = getSortedType();
	switch (option)
	{
	case eByName:
		qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compareProductByName);
		pMarket->productArrSortBy = eByName;
		break;

	case eByCount:
		qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compareProductByCount);
		pMarket->productArrSortBy = eByCount;
		break;

	case eByPrice:
		qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compareProductByPrice);
		pMarket->productArrSortBy = eByPrice;
		break;
	}
}

void searchProductByAtt(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("\nNo Products yet!\n");
		return;
	}
	eSort attribute = pMarket->productArrSortBy;
	if (attribute == eNumOfSorts)
	{
		printf("Cannot search not sorted!");
		return;
	}
	else
	{
		switch (attribute)
		{
		case eByName:
			productSearchGeneric(pMarket, compareProductByName);
			break;

		case eByPrice:
			productSearchGeneric(pMarket, compareProductByPrice);
			break;

		case eByCount:
			productSearchGeneric(pMarket, compareProductByCount);
			break;
		}
	}
	
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBarcodeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return i;
	}
	return -1;
}

Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return pMarket->productArr[i];
	}
	return NULL;

}


Customer* FindCustomerById(SuperMarket* pMarket, const char* id)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomerById(&pMarket->customerArr[i], id))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}


eSort getSortedType()
{
	int option;

	printf("\n");
	do {
		printf("Please enter one of the following types for sort: \n");
		for (int i = 0; i < eNumOfSorts ; i++)
			printf("%d for %s\n", i, sortOptions[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNumOfSorts);

	getchar();

	return (eSort)option;
}


void	productSearchGeneric(SuperMarket* pMarket, int (*compare)(const void*, const void*))
{
	Product tempPro;
	Product* pTempPro = &tempPro;
	Product** pFound;
	printf("Please write product to search that %s:\n", sortOptions[pMarket->productArrSortBy]);
	if(pMarket->productArrSortBy == eByName)
		scanf("%s", tempPro.name);
	else if (pMarket->productArrSortBy == eByCount)
		scanf("%d", &tempPro.count);
	else 
		scanf("%f", &tempPro.price);

	pFound = (Product**)bsearch(&pTempPro, pMarket->productArr, pMarket->productCount, sizeof(Product*), compare);
	if (!pFound)
		printf("Product not found!\n");
	else
	{
		printf("Product found!\n");
		printProduct(*pFound);
	}
	
}

int		writeAllSuperMarketToBinaryFile(const char* fileName, SuperMarket* pMarket)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp)
		return 0;
	int nameLen = (int)strlen(pMarket->name) + 1;
	if (fwrite(&nameLen, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	if (fwrite(pMarket->name, sizeof(char), nameLen, fp) != nameLen)
	{
		fclose(fp);
		return 0;
	}
	if(!writeProductArrToBinaryFile(fp, pMarket->productArr, pMarket->productCount))
	{
		fclose(fp);
		return 0;
	}
	return 1;
}

SuperMarket*	readAllSuperMarketFromBinaryFile(const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
		return NULL;

	SuperMarket* pMarket = (SuperMarket*)malloc(sizeof(SuperMarket));

	if (!pMarket)
	{
		fclose(fp);
		return NULL;
	}

	pMarket->productArrSortBy = eNumOfSorts;       // Default - Non Sorted!
	pMarket->customerArr = NULL;
	pMarket->customerCount = 0;

	int nameLen;

	if(fread(&nameLen, sizeof(int), 1, fp) != 1)
	{
		free(pMarket);
		fclose(fp);
		return NULL;
	}

	pMarket->name = (char*)malloc(sizeof(char) * nameLen + 1);
	if(!pMarket->name)
	{
		free(pMarket);
		fclose(fp);
		return NULL;
	}

	if(fread(pMarket -> name, sizeof(char), nameLen, fp) != nameLen) 
	{
		free(pMarket->name);
		free(pMarket);
		fclose(fp);
		return NULL;
	}

	pMarket->productArr = readProductArrFromBinaryFile(fp, &pMarket->productCount);

	if (!pMarket->productArr)
	{
		freeProducts(pMarket);
		free(pMarket->name);
		free(pMarket);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return pMarket;
}

int		writeAllCustomersToTextFile(const char* fileName, SuperMarket* pMarket)
{
	FILE* fp;
	fp = fopen(fileName, "w");
	if (!fp)
		return 0;

	fprintf(fp, "%d\n", pMarket->customerCount);            

	for (int i = 0; i < pMarket->customerCount ; i++)
	{
		pMarket->customerArr[i].vTable.writeToTxt(fp, &pMarket->customerArr[i]);
	}
	fclose(fp);
	return 1;
}

Customer* readAllCustomersFromTxtFile(const char* fileName, int* pCustomersCount)
{
	FILE* fp;
	fp = fopen(fileName, "r");
	if (!fp)
		return NULL;

	if (fscanf(fp, "%d", pCustomersCount) != 1)
	{
		fclose(fp);
		return NULL;
	}

	Customer* custArr = (Customer*)calloc(*pCustomersCount , sizeof(Customer));
	if (!custArr)
	{
		fclose(fp);
		return NULL;
	}
	for (int i = 0; i < *pCustomersCount; i++)
	{
	
		if (!readCustomerFromTxt(fp, &custArr[i]))
		{
			freeCustomers(custArr, i);
			fclose(fp);
			return NULL;
		}
		
		int isClubM;
		if(fscanf(fp, "%d", &isClubM) != 1)
		{
			freeCustomers(custArr, i);
			fclose(fp);
			return NULL;
		}
		if (!isClubM)
			initCustomerVTable(&custArr[i]);  
		else
		{
			if (!readClubMemberFromTxt(fp, &custArr[i]))
			{
				freeCustomers(custArr, i);
				fclose(fp);
				return NULL;
			}
		}
	}
	fclose(fp);
	return custArr;
}
