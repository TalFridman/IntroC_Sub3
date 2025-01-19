#pragma once
#include "Customer.h"

typedef struct
{
	int			totalMonths;
	Customer*	pCustomerBase;
}ClubMember;

int		initClubMember(Customer* pCustomer);
void	printClubMemebr(Customer* pCustomer);
float	calculatePrice(ClubMember* pClubMember, int* totalPrice);    //BY REFERENCE
