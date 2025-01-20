#pragma once
struct Customer_;

typedef struct
{
	
	//void	cancelShopping(Customer* pCustomer);	???

	int		(*init)(struct Customer_* pCustomer);
	void	(*print)(const struct Customer_* pCustomer);
	void	(*pay)(struct Customer_* pCustomer);
	void	(*freeObject)(struct Customer_* pCustomer); 


	
}CustomerVTable;

