#pragma once
struct Customer_;

typedef struct
{
	
	//void	pay(Customer* pCustomer);				???
	//void	cancelShopping(Customer* pCustomer);	???
	//void	(*freeObject)(struct Customer_* pCustomer); ???

	int		(*init)(struct Customer_* pCustomer);
	void	(*print)(const struct Customer_* pCustomer);
	
	
}CustomerVTable;