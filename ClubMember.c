#include <stdio.h>
#include <stdlib.h>

#include "ClubMember.h"

int initClubMember(Customer* pCustomer)
{
	int totalMonths = 0;

	if (!initCustomer(pCustomer))
		return 0;

	pCustomer -> pDerived = (ClubMember*)malloc(sizeof(ClubMember));

	if (!pCustomer->pDerived)                                             /// IF NECCESERY TO ADD - || !pClubM     ASK EFRAT!!!!!!!!
	{
		freeCustomer(pCustomer);
		return 0;
	}

	ClubMember* pClubM = (ClubMember*)pCustomer->pDerived;

	printf("Please enter the total months of membership: ");
	do {
		if (totalMonths < 0)
			printf("\nTotal months should be positive! please try again!");
	} while (totalMonths < 0);
	scanf("%d", &totalMonths);

	pClubM->totalMonths = totalMonths;

	initClubMemberVTable(pCustomer);

	pClubM->pCustomerBase = pCustomer;

	return 1;
}

void initClubMemberVTable(Customer* pCustomer)
{
	pCustomer->vTable.init = initClubMember;
	pCustomer->vTable.print = printClubMemebr;
	pCustomer->vTable.pay = payForClubMember;
	pCustomer->vTable.freeObject = freeClubMember;
	//pCustomer->vTable.readFromTxt = readClubMemberFromTxt;
	pCustomer->vTable.writeToTxt = writeClubMemberToTxt;
}

void printClubMemebr(const Customer* pCustomer)
{
	printCustomer(pCustomer);
	const ClubMember* pClubM = (ClubMember*)pCustomer->pDerived;
	printf("Total months of membership: %d\n", pClubM -> totalMonths);
}

float calculatePrice(const ClubMember* pClubMember, float* totalPrice)
{
	float precentsDis;
	if (pClubMember->totalMonths < 24)
		precentsDis = (pClubMember->totalMonths) * 0.1f;
	else if (pClubMember->totalMonths < 60)
		precentsDis = 2.5f + ((float)(pClubMember->totalMonths) / 12) * 0.5f;
	else
		precentsDis = 7.5f;
	*totalPrice -= (*totalPrice) * (precentsDis / 100);
	return precentsDis;
}

void payForClubMember(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	const ClubMember* pClubM = (ClubMember*)pCustomer->pDerived;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	float totalPrice = 	printShoppingCart(pCustomer -> pCart);
	float precentDis = calculatePrice(pClubM, &totalPrice);
	printf("for %s is %.2f, after discount of %.2f %%", pCustomer->name, totalPrice, precentDis);
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}

void freeClubMember(Customer* pCustomer)
{
	if(pCustomer->pDerived)
		free(pCustomer->pDerived);
	freeCustomer(pCustomer);
}

int readClubMemberFromTxt(FILE* fp, Customer* pCustomer)
{
	//int isClubM;
	//readCustomerFromTxt(fp, pCustomer);
	ClubMember* pClubM = (ClubMember*)pCustomer->pDerived;
	if (fscanf(fp, "%d", &pClubM->totalMonths) != 1)
		return 0;
	return 1;
}

void writeClubMemberToTxt(FILE* fp, Customer* pCustomer)
{
	writeCustomerToTxt(fp,pCustomer);
	const ClubMember* pClubM = (ClubMember*)pCustomer->pDerived;
	fprintf(fp, "%d %d", 1, pClubM->totalMonths);
}

