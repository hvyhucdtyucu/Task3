#define MAX_PRODUCTS 20
#define BUFFER_SIZE 1024
#define PORT 6969

typedef struct {
    char description[50];
    float price;
    int item_count;
    int times_requested;
    int times_not_received;
} Product;

typedef struct {
    int total_orders;
    int successful_orders;
    int failed_orders;
    float total_revenue;
} Summary;

// Function declarations
void initialize_catalog(Product *catalog);
void handle_order(int client_socket, Product *catalog, Summary *summary);
void generate_report(Product *catalog, Summary summary);
