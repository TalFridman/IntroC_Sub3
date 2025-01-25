# SuperMarket Management System

C-based supermarket management platform designed to handle customer interactions, product management, and checkout processes. This project was developed as part of a software engineering course intro to C.

## Table of Contents
- [Description](#description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Description
The SuperMarket Management System allows users to manage products, register customers, track shopping carts, and handle purchases. It emphasizes efficient memory management and modular code structure.

## Features
- **Customer Management**: Register customers with unique IDs and names, manage their shopping carts, and handle customer-specific purchases. there is customers & club members. club member have discount of total bill. (with PolyMor)
- **Product Management**: Add and update products, display all products, and filter products by type.
- **Shopping Cart**: Add items to shopping carts, update quantities, and process payments.
- **Memory Management**: Dynamic memory allocation and deallocation to prevent memory leaks.
- **Files** : The supermarket read&write from and to binary file. Customers arr read&write from and to text file.
- **Interactive Menu**: Options include:

  - `0) Show super market`
  - `1) Add product`
  - `2) Add customer`
  - `3) Customer shopping`
  - `4) Print shopping cart`
  - `5) Customer shopping cart managment`
  - `6) Print product by type`
  - `7) Sort products by attribute`
  - `8) Search product by attribute`
  - `-1) Exit`

## Installation
1. Clone the repository:
    ```bash
    git clone https://github.com/TalFridman/IntroC_Sub3.git
    ```
2. Navigate to the project directory.

## Project Structure
- `manager.c / manager.h` - Main menu and high-level control.
- `superMarket.c / superMarket.h` - Supermarket-level management, including customer and product handling.
- `product.c / product.h` - Product attributes and operations.
- `shoppingCart.c / shoppingCart.h` - Shopping cart functionality for customers.
- `shoppingItem.c / shoppingItem.h` - Management of individual shopping items.
- `customer.c / customer.h` - Customer registration and interaction.
- `date.c / date.h` - Date validation and formatting.
- `functions.c / functions.h` - Utility functions such as safeRealloc, string handling, and validation.
- `ClubMemebr.c / ClubMember.h` - club member that extend from customer and have another type of membership months.
- `list.c / list.h` - function of linked lists.
- `def.h` - data of struct Node
- `customerVTable.h` - vTable of functions for polymor 

## Contact
For questions or support, contact dorzhavian@gmail.com / talman222@gmail.com

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## All rights reserved to Dor Zhavian & Tal Fridman 
