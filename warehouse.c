#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_AMOUNT_PRODUCT 100
#define WORDLENGTH 20

typedef struct {

  char productName[WORDLENGTH+1];
  int productNum;
  int goodsBalance;

} Product;

void createNewFile(const char fileName[]);
Product createProduct(int productNum, char productName[], int goodsBalance);
void warehouseManipulation(Product warehouse[], const char fileName[], int numOfProducts);
int registerProduct(Product warehouse[], int numOfProducts);
void printHeader();
void printProduct(Product warehouse[], int i);
void viewWarehouse(Product warehouse[], int numOfProducts);
void searchThroughWarehouse(Product warehouse[], int numOfProducts);
void changeGoodsBalance(Product warehouse[], int numOfProducts);
void searchAfterProductNum(Product warehouse[], int numOfProducts);
void searchAfterGoodsBalance(Product warehouse[], int numOfProducts);
void searchAfterProductName(Product warehouse[], int numOfProducts);
void sortThroughWarehouse(Product warehouse[], int numOfProducts);
void sortByProductNum(Product warehouse[], int numOfProducts);
void sortByProductName(Product warehouse[], int numOfProducts);
void sortByGoodsBalance(Product warehouse[], int numOfProducts);
int unregisterProduct(Product warehouse[], int numOfProducts);
void printWarehouseToFile(Product warehouse[], const char fileName[], int numOfProducts);

int main(void) {

  Product warehouse[MAX_AMOUNT_PRODUCT];
  FILE *fp;

  char fileName[WORDLENGTH+1];

  for(;;) {

    char productName[WORDLENGTH+1];
    int productNum, goodsBalance;
    int numOfProducts = 0;

    printf("Ange fil (q - quit): ");
    scanf("%s%*c", &fileName);

    if(fileName[0] == 'q') {
      printf("Avslutar..");
      break;
    }

    fp = fopen(fileName, "r");
    if (fp == NULL) {

      char answer[3+1];
      printf("%s existerar inte.\nSkapar fil %s (ja/nej)?: ", fileName, fileName);
      scanf("%s", &answer);

      if(strlen(answer) == 2) {

        printf("Skapar..\n");
        createNewFile(fileName);
        printf("%s har skapats..\n", fileName);
        warehouseManipulation(warehouse, fileName, numOfProducts);

      }

    } else {

      while(fscanf(fp, "%d %s %d", &productNum, productName, &goodsBalance)==3) {
        warehouse[numOfProducts++] = createProduct(productNum, productName, goodsBalance);
      }

      warehouseManipulation(warehouse, fileName, numOfProducts);

    }
    fclose(fp);

  }

  return 0;

}

void createNewFile(const char fileName[]) {

  FILE *fp;
  fp = fopen(fileName, "w");
  fclose(fp);

}

Product createProduct(int productNum, char productName[], int goodsBalance) {

  Product p;

  p.productNum = productNum;
  strcpy(p.productName, productName);
  p.goodsBalance = goodsBalance;

  return p;

}

void warehouseManipulation(Product warehouse[], const char fileName[], int numOfProducts) {

  int enteredValue;

  for(;;) {

    printf("(1) Registrera vara (2) Skriv ut vara (3) Sok vara (4) Andra lagersaldo ");
    printf("(5) Sortera varor (6) Avregistrera varor (7) Avsluta program\n");
    printf("Ange ett alternativ: ");
    scanf("%d", &enteredValue);

    switch(enteredValue) {

      case 1:
        numOfProducts = registerProduct(warehouse, numOfProducts);
        break;
      case 2:
        if (numOfProducts > 0) {
          viewWarehouse(warehouse, numOfProducts);
        } else {
          printf("Lagret ar tomt..\n");
        }
        break;
      case 3:
        if (numOfProducts > 0) {
          searchThroughWarehouse(warehouse, numOfProducts);
        } else {
          printf("Lagret ar tomt..\n");
        }
        break;
      case 4:
        if (numOfProducts > 0) {
          changeGoodsBalance(warehouse, numOfProducts);
        } else {
          printf("Lagret ar tomt..\n");
        }
        break;
      case 5:
        if (numOfProducts > 0) {
          sortThroughWarehouse(warehouse, numOfProducts);
        } else {
          printf("Lagret ar tomt..\n");
        }
        break;
      case 6:
        if (numOfProducts > 0) {
         numOfProducts = unregisterProduct(warehouse, numOfProducts);
        } else {
          printf("Lagret ar tomt..\n");
        }
        break;
      case 7:
        printf("Avslutar programet..\n");
        printWarehouseToFile(warehouse, fileName, numOfProducts);
        return;
      default:
        printf("Fel inmatning..\n");
        break;

    }

  }

}

int registerProduct(Product warehouse[], int numOfProducts) {

  char productName[WORDLENGTH+1];
  int productNum, goodsBalance;

  for(;;) {

    if (numOfProducts > MAX_AMOUNT_PRODUCT - 1) {
      printf("Lagret ar fullt\n");
      return numOfProducts;
    }

    bool ifUniqeNum = true;

    printf("Ange varunummer (0 for avslut): ");
    int isNum = scanf("%d", &productNum);

    if(!isNum) {

      char tmp[WORDLENGTH+1];
      scanf(" %s", &tmp);
      printf("Fel inmatning..\n");
      continue;

    }

    if(productNum == 0) {
      return numOfProducts;
    }

    if(productNum < 0) {
      printf("Fel inmatning..\n");
      continue;
    }

    //Determine if product number is taken or not. true = taken, false = not taken.
    for(int i = 0; i < numOfProducts; i++) {
      if(warehouse[i].productNum == productNum) {
        ifUniqeNum = false;
      }
    }

    //Product nummer is not taken
    if(ifUniqeNum) {

      for(;;) {

        printf("Ange namn: ");
        scanf("%s%*c", productName);

        //Determine the length of string
        if(strlen(productName) > WORDLENGTH) {
          printf("Overstigt antal karaktarer..\n");
        } else {
          break;
        }

      }

      printf("Ange saldo: ");
      scanf("%d", &goodsBalance);

      if(goodsBalance < 0) {
        goodsBalance = 0;
      }

      warehouse[numOfProducts++] = createProduct(productNum, productName, goodsBalance);

    } else {
      printf("Ej unikt nummer!\n");
    }

  }

  return numOfProducts;

}

void printHeader() {

  printf("Varunummer\tNamn\tLagersaldo\n");
  printf("----------------------------------\n");

}

void printProduct(Product warehouse[], int i) {

  printf("%d\t\t%s\t", warehouse[i].productNum, warehouse[i].productName);
  printf("%d\n", warehouse[i].goodsBalance);

}

void viewWarehouse(Product warehouse[], int numOfProducts) {

  printHeader();
  for(int i = 0; i < numOfProducts; i++) {
    printProduct(warehouse, i);
  }

}

void searchThroughWarehouse(Product warehouse[], int numOfProducts) {

  int enteredValue;

  for(;;) {

    printf("(1) Sok efter varunummer (2) Sok efter namn, ");
    printf("(3) Sok efter lagersaldo (4) Atervand till huvudmenyn\n");
    printf("Ange ett alternativ: ");
    int ifNum = scanf("%d", &enteredValue);

    if(!ifNum) {

      char tmp[WORDLENGTH+1];
      scanf(" %s", &tmp);
      printf("Fel inmatning..\n");
      continue;

    }

    switch (enteredValue) {

      case 1:
        searchAfterProductNum(warehouse, numOfProducts);
        break;
      case 2:
        searchAfterProductName(warehouse, numOfProducts);
        break;
      case 3:
        searchAfterGoodsBalance(warehouse, numOfProducts);
        break;
      case 4:
        return;
      default:
        printf("Fel inmatning..\n");
        break;

    }

  }

}

void searchAfterProductNum(Product warehouse[], int numOfProducts) {

  int productNum;

  for(;;) {

    bool ifProductNumExist = false;

    printf("Ange varunummer (0 for avslut): ");
    int ifNum = scanf("%d", &productNum);

    if(!ifNum) {

      char tmp[WORDLENGTH+1];
      scanf(" %s", tmp);
      printf("Fel inmatning..\n");
      continue;

    }

    if(productNum == 0) {
      break;
    }

    printHeader();
    for(int i = 0; i < numOfProducts; i++) {

      if(warehouse[i].productNum == productNum) {
        printProduct(warehouse, i);
        ifProductNumExist = true;
        break;
      }

    }

    if(!ifProductNumExist) {
      printf("Varunummer matchar inte din sokning..\n");
    }

  }

}

void searchAfterGoodsBalance(Product warehouse[], int numOfProducts) {

  int goodsBalance;

  for(;;) {

    bool ifGoodsBalanceExist = false;

    printf("Ange lagersaldo ( < 0 for avslut): ");
    scanf("%d", &goodsBalance);

    if(goodsBalance < 0) {
      break;
    }

    printHeader();
    for(int i = 0; i < numOfProducts; i++) {

      if(warehouse[i].goodsBalance == goodsBalance) {
        printProduct(warehouse, i);
        ifGoodsBalanceExist = true;
      }

    }

    if(!ifGoodsBalanceExist) {
      printf("Lagersaldot matchar inte din sokning..\n");
    }

  }

}

void searchAfterProductName(Product warehouse[], int numOfProducts) {

  char productName[WORDLENGTH+1];

  for(;;) {

    bool ifProductNameExist = false;

    printf("Ange namn (q for avslut): ");
    scanf("%s", productName);

    if(productName[0] == 'q') {
      break;
    }

    //Comparing the similarity with two strings
    printHeader();
    for(int i = 0; i < numOfProducts; i++) {

      int k = 0;
      for(int j = 0; j < WORDLENGTH; j++) {

        if(warehouse[i].productName[j] == productName[k]) {
          k++;
        } else {
          k = 0;
        }

        if(strlen(productName) == k) {
          printProduct(warehouse, i);
          ifProductNameExist = true;
          break;
        }

      }

    }

    if(!ifProductNameExist) {
      printf("Namnet matchar inte din sokning..\n");
    }

  }

}

void changeGoodsBalance(Product warehouse[], int numOfProducts) {

  int productNum;
  int changeGoodsBalance;
  bool ifProductNumExist = false;

  for(;;) {

    printf("Ange varunummer (0 for avslut): ");
    int ifNum = scanf("%d", &productNum);

    if(!ifNum) {

      char tmp[WORDLENGTH+1];
      scanf(" %s", &tmp);
      printf("Fel inmatning..\n");
      continue;

    }

    if(productNum == 0) {
      break;
    }

    for(int i = 0; i < numOfProducts; i++) {

      if(warehouse[i].productNum == productNum) {
        ifProductNumExist = true;
      }

    }

    if(!ifProductNumExist) {
      printf("Varunummret matchar inte din sokning..\n");
      continue;
    }

    printf("Ange andring av lagersaldo: ");
    scanf("%d", &changeGoodsBalance);

    for(int i = 0; i < numOfProducts; i++) {

      if(warehouse[i].productNum == productNum) {

        warehouse[i].goodsBalance += changeGoodsBalance;

        if(warehouse[i].goodsBalance < 0) {
          printf("Lagersaldot kom under noll..\n");
          warehouse[i].goodsBalance = 0;
        }

        break;

      }

    }

    break;

  }

}

void sortThroughWarehouse(Product warehouse[], int numOfProducts) {

  int enteredValue;

  printf("(1) Sortera efter varunummer (2) Sortera efter namn ");
  printf("(3) Sortera efter lagersaldo (4) Atervand till huvudmenyn\n");
  printf("Ange ett alternativ: ");
  int ifNum = scanf("%d", &enteredValue);

  if(!ifNum) {

    char tmp[WORDLENGTH+1];
    scanf(" %s", &tmp);
    printf("Fel inmatning..\n");
    return;

  }

  switch(enteredValue) {

    case 1:
      sortByProductNum(warehouse, numOfProducts);
      break;
    case 2:
      sortByProductName(warehouse, numOfProducts);
      break;
    case 3:
      sortByGoodsBalance(warehouse, numOfProducts);
    case 4:
      return;
    default:
      printf("Fel inmatning..\n");
      break;

  }

}

void sortByProductNum(Product warehouse[], int numOfProducts) {

  Product tmp;

  for(int i = 0; i < numOfProducts; i++) {

    for(int j = 0; j < numOfProducts - 1; j++) {

      if(warehouse[j].productNum > warehouse[j+1].productNum) {

        tmp = warehouse[j];
        warehouse[j] = warehouse[j+1];
        warehouse[j+1] = tmp;

      }

    }

  }

}

void sortByProductName(Product warehouse[], int numOfProducts) {

  Product tmp;

  for(int i = 0; i < numOfProducts; i++) {

    for(int j = 0; j < numOfProducts - 1; j++) {

      if(strcmp(warehouse[j].productName, warehouse[j+1].productName) > 0) {

        tmp = warehouse[j];
        warehouse[j] = warehouse[j+1];
        warehouse[j+1] = tmp;

      }

    }

  }

}

void sortByGoodsBalance(Product warehouse[], int numOfProducts) {

  Product tmp;

  for(int i = 0; i < numOfProducts; i++) {

    for(int j = 0; j < numOfProducts - 1; j++) {

      if(warehouse[j].goodsBalance > warehouse[j+1].goodsBalance) {

        tmp = warehouse[j];
        warehouse[j] = warehouse[j+1];
        warehouse[j+1] = tmp;

      }

    }

  }
  
}

int unregisterProduct(Product warehouse[], int numOfProducts) {

  int productNum;

  for(;;) {

    bool ifProductNumExist = false;
    int warehouseNumber = 0;

    printf("Ange varunummer (0 for avslut): ");
    int ifNum = scanf("%d", &productNum);

    if(!ifNum) {
      
      char tmp[WORDLENGTH+1];
      scanf(" %s", tmp);
      printf("Fel inmatning..\n");
      continue;

    }

    if(productNum == 0) {
      return numOfProducts;
    }

    //Determine wich element the product is placed
    for(int i = 0; i < numOfProducts; i++) {

        if(productNum == warehouse[i].productNum) {
          warehouseNumber = i;
          ifProductNumExist = true;
          break;
        }

    }

    if(ifProductNumExist) {

      for(warehouseNumber; warehouseNumber < numOfProducts - 1; warehouseNumber++) {
        warehouse[warehouseNumber] = warehouse[warehouseNumber+1];
      }

      return --numOfProducts;

    } else {
      printf("Varunummer matchar inte din sokning..\n");
    }

  }

}

void printWarehouseToFile(Product warehouse[], const char fileName[], int numOfProducts) {

  FILE *fp;
  fp = fopen(fileName, "w");
  for(int i = 0; i < numOfProducts; i++) {
    fprintf(fp, "%d %s %d\n", warehouse[i].productNum, warehouse[i].productName, warehouse[i].goodsBalance);
  }
  fclose(fp);

  printf("Sparar %s\n", fileName);

}