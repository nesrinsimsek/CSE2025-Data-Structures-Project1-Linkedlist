/*
	Elif Nur  Kemiksiz	100217006
	Nesrin    Simsek	150119664
	Reyta Gul Muran 	150117028

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 30
typedef enum { false, true } bool;

struct product {
	int ID;
    char name[MAX_LEN];
	char category[MAX_LEN];
	int price;
	struct product *product_next;
};
typedef struct product ProductNode;
typedef ProductNode* ProductNodePtr;

struct basket {
	int ID;
    int amount;
	struct basket *basket_next;
	struct product *product_list;
};
typedef struct basket BasketNode;
typedef BasketNode* BasketNodePtr;

struct customer {
	int ID;
    char name[MAX_LEN];
	char surname[MAX_LEN];
    struct customer *customer_next;
	struct basket *basket_list;
};
typedef struct customer CustomerNode;
typedef CustomerNode* CustomerNodePtr;

void product_bought_from(CustomerNodePtr customer, int productID) {

	int boughtCheck = 0;
	while(customer != NULL){
		BasketNodePtr basket = customer->basket_list;
		while(basket != NULL){
			ProductNodePtr product = basket->product_list;
			while(product != NULL && product->ID != productID) {
				product = product->product_next;
			}
			if(product != NULL) {
				boughtCheck = 1;	
				printf("\n%s %s", customer->name, customer->surname);
				break;
			}
			basket = basket->basket_next;		
		}
		customer = customer->customer_next;
	}
	if(boughtCheck == 0) printf("Nobody bought this product!"); 
	printf("\n\n");

}

void remove_customer(char name[MAX_LEN], char surname[MAX_LEN], CustomerNodePtr *customer_head) {
	
	CustomerNodePtr previousPtr = NULL;
	CustomerNodePtr currentPtr = *customer_head;
	while(currentPtr != NULL && strcmp(name, currentPtr->name) != 0 && strcmp(surname, currentPtr->surname) != 0) { 
		previousPtr = currentPtr;
		currentPtr = currentPtr->customer_next;
	}
	if (previousPtr == NULL) {
		*customer_head = currentPtr->customer_next;
	}
	else if(currentPtr == NULL)
		puts("Invalid customer name and surname.\n");
	else {
		previousPtr->customer_next = currentPtr->customer_next;
	}
	
}

void insert_alphabetical_product(ProductNodePtr *product_head, ProductNodePtr prd ) {
	
	ProductNodePtr previousPtr = NULL;
	ProductNodePtr currentPtr = *product_head;	
	while (currentPtr != NULL && strcmp(prd->name, currentPtr->name)>0) { 
		previousPtr = currentPtr;
		currentPtr = currentPtr->product_next;
	}	
	if (previousPtr == NULL) {
		prd->product_next = *product_head;
		*product_head = prd;
	}else {
		previousPtr->product_next = prd;
		prd->product_next = currentPtr;
	}
	
}

int insert_customer(CustomerNodePtr cst, CustomerNodePtr *customer_head){
	
	CustomerNodePtr previousPtr = NULL;
	CustomerNodePtr currentPtr = *customer_head;
	if(cst->ID == -1) {
		while(currentPtr != NULL) { 
			if(strcmp(cst->name, currentPtr->name) == 0 && strcmp(cst->surname, currentPtr->surname) == 0) return 0;
			previousPtr = currentPtr;
			currentPtr = currentPtr->customer_next;
		}
		cst->ID = previousPtr->ID + 1;
	}else {
		while(currentPtr != NULL && cst->ID > currentPtr->ID) { 
			previousPtr = currentPtr;
			currentPtr = currentPtr->customer_next;
		}
	}
			
	if (previousPtr == NULL) {
		cst->customer_next = *customer_head;
		*customer_head = cst;
	}else {
		previousPtr->customer_next = cst;
		cst->customer_next = currentPtr;
	}
	
	return 1;
	
}

int insert_basket (int basketCustomerID, BasketNodePtr basket, CustomerNodePtr *customer_head) {
	
	CustomerNodePtr currentPtr = *customer_head;
	basket->basket_next = NULL;
	
	while(currentPtr != NULL && currentPtr ->ID != basketCustomerID) { 
		currentPtr = currentPtr->customer_next;
	}
		
	BasketNodePtr basketPtr = currentPtr->basket_list;	
	while(basketPtr != NULL){ 
		if(basketPtr->ID == basket->ID)	return 0;		
		basketPtr = basketPtr->basket_next; 
	}
	
	if(currentPtr->basket_list == NULL){
		if(basket->ID == -1) basket->ID = 1;
		currentPtr->basket_list = basket;
	}else {	
	    BasketNodePtr currentBasketPtr = currentPtr->basket_list;			
		while(currentBasketPtr != NULL){			
			if(currentBasketPtr->basket_next == NULL) break;
			currentBasketPtr = currentBasketPtr->basket_next;
		}		
		currentBasketPtr->basket_next = basket;					
		if(basket->ID == -1) basket->ID = currentBasketPtr->ID + 1; 			
	}
			
	return 1;
	
}

int add_product(int custIDToAddProduct, int productIDToAdd, BasketNodePtr basketPtr, char menuChoice, ProductNodePtr *basketProductList, 
				ProductNodePtr currentProductPtr, ProductNodePtr *product_list, CustomerNodePtr currentCustPtr) {
	
	if(productIDToAdd == -1) return 0;	
	
	ProductNodePtr productPtr = *product_list;	
	*basketProductList = *product_list;
	
	while(currentCustPtr != NULL && currentCustPtr->ID != custIDToAddProduct) { 
		currentCustPtr = currentCustPtr->customer_next;
	}
	while(currentProductPtr != NULL && currentProductPtr->ID != productIDToAdd) { 
		currentProductPtr = currentProductPtr->product_next;
	}

	ProductNodePtr productWillBeAdded = malloc(sizeof(struct product));
	productWillBeAdded->ID = productIDToAdd;
	strcpy(productWillBeAdded->name, currentProductPtr->name);
	strcpy(productWillBeAdded->category, currentProductPtr->category);
	productWillBeAdded->price = currentProductPtr->price;
	productWillBeAdded->product_next = NULL;
	
	if(menuChoice == '2') printf("\nThe products in the basket:\n");
	
	if(productPtr == NULL) {
		*product_list = productWillBeAdded;
	}else {
		while(productPtr != NULL) {
			if(productPtr->product_next == NULL) break;
			productPtr=productPtr->product_next;
		}productPtr->product_next = productWillBeAdded;
	}
	
	basketPtr->amount = 0;
	ProductNodePtr currentProductInBasket = *product_list;	
	while(currentProductInBasket != NULL) {		
		basketPtr->amount += currentProductInBasket->price;	
		if(menuChoice == '2') printf("%d %s %s %d\n", currentProductInBasket->ID, currentProductInBasket->name, 
										currentProductInBasket->category, currentProductInBasket->price);
		currentProductInBasket = currentProductInBasket->product_next;		
	}	
	if (menuChoice == '2') printf("Total amount of %s %s's #%d basket is $%d.\n", currentCustPtr->name, currentCustPtr->surname, 
									basketPtr->ID, basketPtr->amount);
	
	return 1;
		
}

void list_total_shopping_amount(CustomerNodePtr customer) {
		
	int amountOfBasket=0;
	printf("\n");
	while(customer != NULL) {	
		BasketNodePtr basket = customer->basket_list;			
		if(basket == NULL) {
			printf("%s %s did not buy anything.\n",customer->name, customer->surname);
			customer = customer->customer_next;
			continue;
		}	
		while(basket!=NULL) {		
			amountOfBasket += basket->amount;	
			basket = basket->basket_next;
		}
		printf("Total amount of %s %s's shopping is $%d.\n", customer->name, customer->surname, amountOfBasket);	
		customer = customer->customer_next;
		amountOfBasket = 0;
	}
	printf("\n");
	
}

void print_product(ProductNodePtr product_head) {
	
	puts("\nThe list of products:");
	while (product_head != NULL) {
		printf("%d\t%s\t%s\t%d\n", product_head->ID, product_head->name, product_head->category, product_head->price);
		product_head = product_head->product_next;
	}
	
}

void print_customer(CustomerNodePtr customer_head) {
	
	puts("\nThe list of customers:");
	while (customer_head != NULL) {
		printf("%d\t%s\t%s\n", customer_head->ID, customer_head->name, customer_head->surname);
		customer_head = customer_head->customer_next;
	}
	
}

void print_options(void) {
	  
	  printf(
      "   1 to insert a customer into the list.\n"
      "   2 to insert a basket into the customer account.\n"
      "   3 to remove customer from the list.\n"
	  "   4 to print list the customers who bought a specific product.\n"
	  "   5 to print list the total shopping amounts of each customer.\n"
	  "   6 to exit.\n"
	  "Enter your choice: ");
	  
}
 
int main(int argc, char *argv[]) {
	
	CustomerNodePtr customer_head = NULL;
    ProductNodePtr product_head = NULL;

	FILE *fp;
	char name[MAX_LEN], surname[MAX_LEN];
	int ID;
	char id[MAX_LEN];
	fp = fopen("customer.txt", "r");
	while (fscanf(fp, "%s\t%s\t%s\n", id, name, surname) != EOF) {
		
		ID = atoi(id);
		CustomerNodePtr cst = malloc(sizeof(struct customer));
		 
		if (cst == NULL) {
			puts("Memory allocation failed!");
			exit(-1);
		}
		
		strcpy(cst->name, name);
		strcpy(cst->surname, surname);
		cst->ID = ID;		
		cst->basket_list = NULL;
		cst->customer_next = NULL;
		insert_customer(cst,&customer_head);
		
	}	
	fclose(fp);
	
	FILE *basketPtr, *basketPtr2;
	int basketCustomerID, basketID, basketProductID;
	char basketcustomerid[MAX_LEN], basketid[MAX_LEN], basketproductid[MAX_LEN];
	basketPtr = fopen("basket.txt", "r");
	basketPtr2 = fopen("basket.txt", "r");		
	while (fscanf(basketPtr, "%s\t%s\t%s\n", basketcustomerid, basketid, basketproductid) != EOF) {     
		
		basketCustomerID = atoi(basketcustomerid);
		basketID = atoi(basketid);
		basketProductID = atoi(basketproductid);
		
		BasketNodePtr basket = malloc(sizeof(struct customer)); 	
		if (basket == NULL) {
			puts("Memory allocation failed!");
			exit(-1);
		}
		
		basket->ID = basketID;
		basket->basket_next = NULL;
		basket->product_list = NULL; 
		
		insert_basket(basketCustomerID, basket, &customer_head);
				
	}
	fclose(basketPtr);
		
	FILE *prod;
	char product_name[MAX_LEN], category[MAX_LEN];
	int product_ID, price;
	char p_id[MAX_LEN], p_price[MAX_LEN];	
	prod = fopen("product.txt", "r");	
	while (fscanf(prod, "%s\t%s\t%s\t%s\n", p_id, product_name, category, p_price) != EOF) {
		
		product_ID = atoi(p_id);
		price = atoi(p_price);

		ProductNodePtr prd = malloc(sizeof(ProductNode)); 
		if (prd == NULL) {
			puts("Memory allocation failed!");
			exit(-1);
		}
		
		prd->ID = product_ID;
		strcpy(prd->name, product_name);
		strcpy(prd->category, category);
		prd->price = price;
		prd->product_next = NULL;

		insert_alphabetical_product(&product_head, prd);
		
	}
	fclose(prod);

	ProductNodePtr productList = malloc(sizeof(struct product));
	productList->product_next = NULL;	
	int sameBasketCheck = 0, sameCustomerCheck = 0;
	while (fscanf(basketPtr2, "%s\t%s\t%s\n", basketcustomerid, basketid, basketproductid) != EOF) { 
		
		basketCustomerID = atoi(basketcustomerid);
		basketID = atoi(basketid);
		basketProductID = atoi(basketproductid);
		
		CustomerNodePtr currentCustPtr = customer_head;
		ProductNodePtr currentProductPtr = product_head;
		
		if(sameBasketCheck != basketID || sameCustomerCheck != basketCustomerID ) productList = NULL;
			
		while(currentCustPtr != NULL && currentCustPtr->ID!= basketCustomerID) { 
				currentCustPtr = currentCustPtr->customer_next;
		}

		BasketNodePtr basket = currentCustPtr->basket_list;
		while(basket != NULL && basket->ID != basketID ) {
			if(basket->basket_next == NULL) break;
			basket = basket->basket_next;		
		}
	 	
        add_product(basketCustomerID, basketProductID, basket, '0', &(basket->product_list), product_head, &productList, customer_head);
        
		sameBasketCheck = basketID;
		sameCustomerCheck = basketCustomerID; 
		     	
	}
	fclose(basketPtr2);
	
	char choice = ' ';	
	while(true) {		
		print_options();
		scanf(" %c",&choice);		
		
		if(choice == '1') {
			
			print_customer(customer_head);       
			char name[MAX_LEN], surname[MAX_LEN];
            
			CustomerNodePtr cst = malloc(sizeof(struct customer));
            printf("Enter new customer's Name and Surname: ");
            scanf(" %s %s", &name, &surname);       
            strcpy(cst->name, name);
            strcpy(cst->surname, surname);
            cst->ID = -1;
            
			while(!insert_customer(cst, &customer_head)) {
            	printf("Customer Name and Surname must be unique! Enter again: ");
            	scanf(" %s %s", &name, &surname);
            	strcpy(cst->name, name);
            	strcpy(cst->surname, surname);
			}
            
			cst->basket_list = NULL;
            cst->customer_next = NULL;	
			print_customer(customer_head);	
			
        }
     
		if(choice == '2') {
			
			print_customer(customer_head);			
			BasketNodePtr basket = malloc(sizeof(struct basket));
			basket->basket_next = NULL;
			basket->product_list = NULL;
			basket->ID = -1;
			ProductNodePtr productList = malloc(sizeof(struct product));
			productList->product_next = NULL;
			productList = NULL;		
			
        	int custIDToAddBasket, productIDToAdd;
        	printf("Enter the Customer ID to add basket: ");
        	scanf(" %d", &custIDToAddBasket); 	
        	insert_basket(custIDToAddBasket, basket, &customer_head);
        	print_product(product_head);
        	printf("\nEnter the Product ID you want to add:\nEnter -1 to finish! ");
        	scanf(" %d", &productIDToAdd);
        	
        	while(productIDToAdd != -1) {
        		add_product(custIDToAddBasket, productIDToAdd, basket, choice, &(basket->product_list), 
							product_head, &productList, customer_head);      		
        		printf("\nEnter the Product ID you want to add:\nEnter -1 to finish! ");
        		scanf(" %d", &productIDToAdd);
        		
			} 
			        
		}
			
		if(choice == '3') {	
				
			print_customer(customer_head);
            char name[MAX_LEN], surname[MAX_LEN];
            printf("Enter customer's Name and Surname: ");
            scanf(" %s %s", &name, &surname);
            remove_customer(name, surname, &customer_head);
			print_customer(customer_head);
			
        }
        
        if(choice == '4') {
        	
        	int productIDToViewSales;
        	print_product(product_head);
        	printf("\nEnter the Product ID to view which customer bought it: ");
        	scanf(" %d", &productIDToViewSales);
        	product_bought_from(customer_head, productIDToViewSales);
        	
		}
		
        if(choice=='5') {
        
        	list_total_shopping_amount(customer_head);
        	
		}	

		if(choice == '6') {
			
	    	printf("Thank you for shopping. Have a nice Day! ");
	    	break;
	    	
	    }
	
	}
	return 0;
	
}
