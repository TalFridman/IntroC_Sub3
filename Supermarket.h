#pragma once
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"

typedef enum { eByName, eByCount, eByPrice, eNumOfSorts } eSort;


typedef struct
{
	char*		name;
	Customer*	customerArr;
	int			customerCount;
	Product**	productArr;
	int			productCount;
	eSort		productArrSortBy;
}SuperMarket;


int			initSuperMarket(SuperMarket* pMarket);
void		printSuperMarket(const SuperMarket* pMarket);
int			addProduct(SuperMarket* pMarket);
int			addNewProduct(SuperMarket* pMarket);
int			addCustomer(SuperMarket* pMarket);
int			isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int			doShopping(SuperMarket* pMarket);
Customer*	doPrintCart(SuperMarket* pMarket);
int			manageShoppingCart(SuperMarket* pMarket);
Customer*	getCustomerShopPay(SuperMarket* pMarket);
void		fillCart(SuperMarket* pMarket, ShoppingCart* pCart);
void		clearCart(SuperMarket* pMarket, Customer* pCustomer);

void		printProductByType(SuperMarket* pMarket);
void		printAllProducts(const SuperMarket* pMarket);
void		printAllCustomers(const SuperMarket* pMarket);
int			isBarcodeUnique(const SuperMarket* pMarket, const char* barcode);
int 		isCustomerIdUnique(const SuperMarket* pMarket, const char* id);


int			getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductFromUser(SuperMarket* pMarket, char* barcode);
void		getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer*	FindCustomerById(SuperMarket* pMarket, const char* id);

Customer*	getCustomerWhoShop(SuperMarket* pMarket);
Product*	getProductAndCount(SuperMarket* pMarket, int* pCount);

void		freeMarket(SuperMarket* pMarket);
void		freeProducts(SuperMarket* pMarket);
void		freeCustomers(Customer* pCustomersArr, int numOfCustomers);

void	sortProductsByAtt(SuperMarket* pMarket);
eSort	getSortedType();
void	searchProductByAtt(SuperMarket* pMarket);

void	productSearchGeneric(SuperMarket* pMarket, int (*compare)(const void*, const void*));

SuperMarket* readAllSuperMarketFromBinaryFile(const char* fileName);
int		writeAllSuperMarketToBinaryFile(const char* fileName, SuperMarket* pMarket);

int		writeAllCustomersToTextFile(const char* fileName, SuperMarket* pMarket);
Customer* readAllCustomersFromTxtFile(const char* fileName, int* pCustomersCount);
