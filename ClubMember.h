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
