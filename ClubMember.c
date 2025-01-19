#include <stdio.h>
#include <stdlib.h>

#include "ClubMember.h"

int initClubMember(Customer* pCustomer)
{
	if (!initCustomer(pCustomer))
		return 0;
	ClubMember* pClubM = (ClubMember*)malloc(sizeof(ClubMember));
	if (!pCustomer->pDerived)                                       /// IF NECCESERY TO ADD - || !pClubM     ASK EFRAT!!!!!!!!
	{
		freeCustomer(pCustomer);
		return 0;
	}
	pCustomer->pDerived = pClubM;

	printf("Please enter the total months of membership: ");
	scanf("%d", &pClubM->totalMonths);

	pClubM->pCustomerBase = pCustomer;

	pCustomer->vTable.init = initClubMember;
	pCustomer->vTable.print = printClubMemebr;

	return 1;
}

void printClubMemebr(Customer* pCustomer)
{
	printCustomer(pCustomer);
	const ClubMember* pClubM = (ClubMember*)pCustomer->pDerived;
	printf("Total months of membership: %d", pClubM -> totalMonths);
}

float calculatePrice(ClubMember* pClubMember, int* totalPrice)
{
	float precentsDis;
	if (pClubMember->totalMonths < 24)
		precentsDis = (pClubMember->totalMonths) * 0.001;
	else if (pClubMember->totalMonths < 60)
		precentsDis = 0.025 + ((pClubMember->totalMonths) / 12) * 0.005;
	else
		precentsDis = 0.075;
	*totalPrice = (*totalPrice) * precentsDis;
	return precentsDis;
}
