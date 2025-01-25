#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Product.h"
#include "General.h"

#define MIN_DIG 3
#define MAX_DIG 5

static const char* typeStr[eNofProductType] = { "Fruit Vegtable", "Fridge", "Frozen", "Shelf" };
static const char* typePrefix[eNofProductType] = { "FV", "FR", "FZ", "SH" };

void	initProduct(Product* pProduct)
{
	initProductNoBarcode(pProduct);
	generateBarcode(pProduct);
}

void	initProductNoBarcode(Product* pProduct)
{
	initProductName(pProduct);
	pProduct->type = getProductType();
	initDate(&pProduct->expiryDate);
	pProduct->price = getPositiveFloat("Enter product price\t");
	pProduct->count = getPositiveInt("Enter product number of items\t");
}

void initProductName(Product* pProduct)
{
	do {
		printf("enter product name up to %d chars\n", NAME_LENGTH);
		myGets(pProduct->name, sizeof(pProduct->name));
	} while (checkEmptyString(pProduct->name));
}

void	printProduct(const Product* pProduct)
{
	char* dateStr = getDateStr(&pProduct->expiryDate);
	printf("%-20s %-10s\t", pProduct->name, pProduct->barcode);
	printf("%-20s %5.2f %13d %7s %15s\n", typeStr[pProduct->type], pProduct->price, pProduct->count, " ", dateStr);
	free(dateStr);
}

void generateBarcode(Product* pProd)
{
	char temp[BARCODE_LENGTH + 1];
	int barcodeNum;

	strcpy(temp, getProductTypePrefix(pProd->type));
	do {
		barcodeNum = MIN_BARCODE + rand() % (RAND_MAX - MIN_BARCODE + 1); //Minimum 5 digits
	} while (barcodeNum > MAX_BARCODE);
	
	sprintf(temp + strlen(temp), "%d", barcodeNum);

	strcpy(pProd->barcode, temp);
}

void getBarcodeCode(char* code)
{
	char temp[MAX_STR_LEN];
	char msg[MAX_STR_LEN];
	sprintf(msg, "Code should be of %d length exactly\n"
				 "Must have %d type prefix letters followed by a %d digits number\n"
				 "For example: FR20301",
				 BARCODE_LENGTH, PREFIX_LENGTH, BARCODE_DIGITS_LENGTH);
	int ok = 1;
	int digCount = 0;
	do {
		ok = 1;
		digCount = 0;
		getsStrFixSize(temp, MAX_STR_LEN, msg);
		if (strlen(temp) != BARCODE_LENGTH)
		{
			puts("Invalid barcode length");
			ok = 0;
		}
		else
		{
			//check first PREFIX_LENGTH letters are upper case and valid type prefix
			char* typeSubStr = (char*)malloc(PREFIX_LENGTH + 1);
			if (!typeSubStr)
				return;
			strncpy(typeSubStr, temp, PREFIX_LENGTH);
			typeSubStr[PREFIX_LENGTH] = '\0';
			int prefixOk = 0;
			int i;

			for (i = 0; i < eNofProductType; i++)
			{
				if (strcmp(typeSubStr, typePrefix[i]) == 0)
				{
					prefixOk = 1;
					break; //found valid type prefix
				}
			}

			free(typeSubStr); //free the allocated memory

			if (!prefixOk)
			{
				puts("Invalid type prefix");
				ok = 0;
			}
			else
			{
				for (i = PREFIX_LENGTH; i < BARCODE_LENGTH; i++)
				{
					if (!isdigit(temp[i]))
					{
						puts("Only digits after type prefix\n");
						puts(msg);
						ok = 0;
						break;
					}
					digCount++;
				}

				if (digCount != BARCODE_DIGITS_LENGTH)
				{
					puts("Incorrect number of digits");
					ok = 0;
				}
			}
		}
	} while (!ok);

	strcpy(code, temp);
}

eProductType getProductType()
{
	int option;

	printf("\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofProductType; i++)
			printf("%d for %s\n", i, typeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofProductType);

	getchar();

	return (eProductType)option;
}

const char* getProductTypeStr(eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typeStr[type];
}

const char* getProductTypePrefix(eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typePrefix[type];
}

int		isProduct(const Product* pProduct, const char* barcode)
{
	if (strcmp(pProduct->barcode, barcode) == 0)
		return 1;
	return 0;
}

void	updateProductCount(Product* pProduct)
{
	int count;

	do {
		printf("How many items to add to stock? ");
		scanf("%d", &count);
	} while (count < 1);

	pProduct->count += count;
}


void	freeProduct(Product* pProduct)
{
	//nothing to free!!!!
}

int		compareProductByName(const void* pP1, const void* pP2)
{
	const Product* Product1 = *(Product**)pP1;
	const Product* Product2 = *(Product**)pP2;
	return strcmp(Product1->name, Product2->name);

}

int		compareProductByPrice(const void* pP1, const void* pP2)
{
	const Product* Product1 = *(Product**)pP1;
	const Product* Product2 = *(Product**)pP2;
	if (Product1->price > Product2->price)
		return 1;
	else if (Product1->price < Product2->price)
		return -1;
	else
		return 0;
}

int		compareProductByCount(const void* pP1, const void* pP2)
{
	const Product* Product1 = *(Product**)pP1;
	const Product* Product2 = *(Product**)pP2;
	return Product1->count - Product2->count;
}

int		writeProductToBinaryFile(const Product* pProd, FILE* fp)
{
	if (fwrite(pProd, sizeof(Product), 1, fp) != 1)
		return 0;
	return 1;
}

int			readProductFromBinaryFile(Product* pProd, FILE* fp)
{
	if (fread(pProd, sizeof(Product), 1, fp) != 1)
		return 0;
	return 1;
}

int		writeProductArrToBinaryFile(FILE* fp, Product** proArr, int count)
{
	if (fwrite(&count, sizeof(int), 1, fp) != 1)
	{
		return 0;
	}
	for (int i = 0; i < count; i++)
	{
		if (!writeProductToBinaryFile(proArr[i],fp))
		{
			return 0;
		}
	}
	return 1;
}

Product**	readProductArrFromBinaryFile(FILE* fp, int* pCount)
{
	Product** proArr = NULL;
	if (fread(pCount,sizeof(int), 1,fp) != 1)
	{
		return NULL;
	}
	proArr = (Product**)malloc(sizeof(Product*) * (*pCount));
	if (!proArr)
	{
		return NULL;
	}

	for (int i = 0; i < *pCount; i++)
	{
		proArr[i] = (Product*)malloc(sizeof(Product)); 
		if (!proArr[i])
		{
			for (int j = 0; j < i; j++) 
				free(proArr[j]);
			free(proArr);
			return NULL;
		}

		if (!readProductFromBinaryFile(proArr[i], fp))
		{
			for (int j = 0; j <= i; j++)
				free(proArr[j]);
			free(proArr);
			return NULL;
		}
	}
	return proArr;
}