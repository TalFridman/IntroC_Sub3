#pragma once
#include "Customer.h"

typedef struct
{
	int			totalMonths;
	Customer*	pCustomerBase;
}ClubMember;

int		initClubMember(Customer* pCustomer);
void	initClubMemberVTable(Customer* pCustomer);
void	printClubMemebr(const Customer* pCustomer);
float	calculatePrice(const ClubMember* pClubMember, float* totalPrice);    //BY REFERENCE
void	payForClubMember(Customer* pCustomer);
void	freeClubMember(Customer* pCustomer);
void	writeClubMemberToTxt(FILE* fp, Customer* pCustomer);
int		readClubMemberFromTxt(FILE* fp, Customer* pCustomer);
