#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "header.h"

void client_process() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    char buffer[BUFFER_SIZE];

    for (int i = 0; i < 10; i++) { // Each client sends 10 orders
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\nSocket creation error\n");
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            printf("\nInvalid address / Address not supported\n");
            exit(EXIT_FAILURE);
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed\n");
            exit(EXIT_FAILURE);
        }

        int product_id = rand() % MAX_PRODUCTS; // Random product ID
        write(sock, &product_id, sizeof(product_id) + 1); // Send order

        read(sock, buffer, BUFFER_SIZE); // Receive response
        printf("Client (PID %d) received: %s\n", getpid(), buffer);

        close(sock); // Close connection
        sleep(1); // Wait 1 second between orders
    }
}

void initialize_catalog(Product *catalog) {
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        snprintf(catalog[i].description, sizeof(catalog[i].description), "Product %d", i + 1);
        catalog[i].price = (float)((rand() % 1000) / 10.0); // value 0.0~99.9 casted from double to float
        catalog[i].item_count =2;
        catalog[i].times_requested =0;
        catalog[i].times_not_received=0;
    }
}

void handle_order(int client_socket, Product *catalog, Summary *summary) {
    char buffer[BUFFER_SIZE];
    int product_id;

    read(client_socket, &product_id, sizeof(product_id));

    if (product_id < 0 || product_id >= MAX_PRODUCTS) {
        snprintf(buffer, BUFFER_SIZE, "Invalid product ID");
        write(client_socket, buffer, strlen(buffer) + 1);
        summary->failed_orders++;
        return;
    }

    Product *product = &catalog[product_id];

    if (product->item_count > 0) {
        product->item_count--;
        summary->successful_orders++;
        summary->total_revenue += product->price;
        snprintf(buffer, BUFFER_SIZE, "Order successful: %s, $%.2f", product->description, product->price);
    } else {
        summary->failed_orders++;
        product->times_not_received++;
        snprintf(buffer, BUFFER_SIZE, "Out of stock: %s", product->description);
    }
    product->times_requested++;
    write(client_socket, buffer, strlen(buffer) + 1);
    sleep(1); // wait 1 sec
}

void generate_report(Product *catalog, Summary summary) {
    printf("\n--- Report ---\n");
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        printf("Product: %s, Sold: %d, Requested: %d, Not received:%d\n", catalog[i].description, 2 - catalog[i].item_count, catalog[i].times_requested, catalog[i].times_not_received);
    }
    printf("\nSummary:\n");
    printf("Total orders: %d\n", summary.total_orders);
    printf("Successful: %d\n", summary.successful_orders);
    printf("Failed: %d\n", summary.failed_orders);
    printf("Total revenue: $%.2f\n", summary.total_revenue);
}

