#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_SUPPLIERS 10
#define MAX_STORES 10

// ID formatting: always print IDs as exactly 3 digits (001, 012, 123)
#define ID_PAD 3

typedef struct
{
    int id;
    int capacity; // max items for the trucks
} Supplier;

typedef struct
{
    int id;
} Store;

// ===================== TERMINAL =====================

//.      ANSI escape

void clear_screen(void)
{
    printf("\033[2J\033[H");   ///
}

// ===================== UI HELPERS =====================

void print_title(string title)
{
    printf("%s\n\n", title);
}

void back_to_menu_prompt(void)
{
    get_int("\nPress any number to return: ");
}

 // ============ FOR ERRORS ==========

void show_error_and_back(string msg, int warehouse_current, int warehouse_max)
{
    printf("\nERROR: %s\n", msg);
    printf("Warehouse: %d / %d (free: %d)\n",
           warehouse_current,
           warehouse_max,
           warehouse_max - warehouse_current);
    back_to_menu_prompt();
}

// ===================== FIND HELPERS =====================

int find_supplier_index(Supplier suppliers[], int supplier_count, int supplier_id)
{
    for (int i = 0; i < supplier_count; i++)
    {
        if (suppliers[i].id == supplier_id)
        {
            return i;
        }
    }
    return -1;
}

int find_store_index(Store stores[], int store_count, int store_id)
{
    for (int i = 0; i < store_count; i++)
    {
        if (stores[i].id == store_id)
        {
            return i;
        }
    }
    return -1;
}

bool supplier_id_exists(Supplier suppliers[], int count, int id)
{
    return find_supplier_index(suppliers, count, id) != -1;
}

bool store_id_exists(Store stores[], int count, int id)
{
    return find_store_index(stores, count, id) != -1;
}

// ===================== INPUT HELPERS =====================

int prompt_existing_supplier_index(Supplier suppliers[], int supplier_count)
{
    int supplier_id = get_int("\nSupplier ID: ");
    int idx = find_supplier_index(suppliers, supplier_count, supplier_id);

    while (idx == -1)
    {
        printf("ERROR: Supplier not found.\n");
        supplier_id = get_int("Supplier ID: ");
        idx = find_supplier_index(suppliers, supplier_count, supplier_id);
    }
    return idx;
}

int prompt_existing_store_index(Store stores[], int store_count)
{
    int store_id = get_int("\nStore ID: ");
    int idx = find_store_index(stores, store_count, store_id);

    while (idx == -1)
    {
        printf("ERROR: Store not found.\n");
        store_id = get_int("Store ID: ");
        idx = find_store_index(stores, store_count, store_id);
    }
    return idx;
}

// ===================== UI TABLE PRINTS =====================

void print_suppliers_table(Supplier suppliers[], int supplier_count) //for suppliers
{
    printf(" ID  | Max delivery\n");
    printf("-----+-------------\n");

    for (int i = 0; i < supplier_count; i++)
    {
        printf(" %0*d | %11d\n",
               ID_PAD,
               suppliers[i].id,
               suppliers[i].capacity);
    }
}

void print_stores_table(Store stores[], int store_count, int store_quantities[]) // for stores
{
    printf(" ID  | Current qty\n");
    printf("-----+------------\n");

    for (int i = 0; i < store_count; i++)
    {
        printf(" %0*d | %10d\n",
               ID_PAD,
               stores[i].id,
               store_quantities[i]);
    }
}

// ===================== SETTINGS (view only v1) =====================

void settings_menu(Supplier suppliers[], int supplier_count,
                   Store stores[], int store_count,
                   int store_quantities[])
{
    while (true)
    {
        clear_screen();
        print_title("SETTINGS");

        printf("1. View suppliers\n");
        printf("2. View stores\n");
        printf("3. Back\n");

        int choice = get_int("\nChoose option: ");

        switch (choice)
        {
            case 1:
                clear_screen();
                print_title("SUPPLIERS");
                if (supplier_count == 0)
                    printf("No suppliers available.\n");
                else
                    print_suppliers_table(suppliers, supplier_count);
                back_to_menu_prompt();
                break;

            case 2:
                clear_screen();
                print_title("STORES");
                if (store_count == 0)
                    printf("No stores available.\n");
                else
                    print_stores_table(stores, store_count, store_quantities);
                back_to_menu_prompt();
                break;

            case 3:
                return;

            default:
                printf("\nERROR: Invalid option.\n");
                back_to_menu_prompt();
        }
    }
}

// ===================== MAIN =====================

int main(void)
{
    int max_warehouse_capacity = get_int("Max total quantity warehouse can hold (>=0): ");
    while (max_warehouse_capacity < 0)
        max_warehouse_capacity = get_int("Max total quantity warehouse can hold (>=0): ");

    int supplier_count = get_int("How many suppliers do you have (0-10): ");
    while (supplier_count < 0 || supplier_count > MAX_SUPPLIERS)
        supplier_count = get_int("How many suppliers do you have (0-10): ");

    Supplier suppliers[MAX_SUPPLIERS];
    for (int i = 0; i < supplier_count; i++)
    {
        printf("\nSupplier %d/%d\n", i + 1, supplier_count);

        int id = get_int("  Supplier ID: ");
        while (id <= 0 || supplier_id_exists(suppliers, i, id))
            id = get_int("  Supplier ID: ");

        suppliers[i].id = id;

        int cap = get_int("  Max delivery (>=0): ");
        while (cap < 0)
            cap = get_int("  Max delivery (>=0): ");

        suppliers[i].capacity = cap;
    }

    int store_count = get_int("How many stores do you have (0-10): ");
    while (store_count < 0 || store_count > MAX_STORES)
        store_count = get_int("How many stores do you have (0-10): ");

    Store stores[MAX_STORES];
    int store_quantities[MAX_STORES];

    for (int i = 0; i < store_count; i++)
    {
        printf("\nStore %d/%d\n", i + 1, store_count);

        int id = get_int("  Store ID: ");
        while (id <= 0 || store_id_exists(stores, i, id))
            id = get_int("  Store ID: ");

        stores[i].id = id;
        store_quantities[i] = 0;
    }

    int warehouse_current = 0;

    while (true)
    {
        clear_screen();
        print_title("MAIN MENU");

        printf("Warehouse: %d / %d (free: %d)\n\n",
               warehouse_current,
               max_warehouse_capacity,
               max_warehouse_capacity - warehouse_current);

        printf("1. Record delivery to warehouse\n");
        printf("2. Record delivery to stores\n");
        printf("3. Warehouse screen\n");
        printf("4. Settings\n");
        printf("5. Exit\n");

        int choice = get_int("\nChoose option: ");

        switch (choice)
        {
            case 1:
            {
                clear_screen();
                print_title("DELIVERY TO WAREHOUSE");

                /// shows error if there's no WH
                if (supplier_count == 0)
                {
                    show_error_and_back("No suppliers available.", warehouse_current, max_warehouse_capacity);
                    break;
                }

                print_suppliers_table(suppliers, supplier_count);

                int sindex = prompt_existing_supplier_index(suppliers, supplier_count);
//
                int amount = get_int("Amount delivered (>0): ");
                while (amount <= 0)
                {
                    printf("ERROR: Amount must be positive.\n");
                    amount = get_int("Amount delivered (>0): ");
                }

                if (amount > suppliers[sindex].capacity)
                {
                    show_error_and_back("Supplier max delivery exceeded.", warehouse_current, max_warehouse_capacity);
                    break;
                }

                if (warehouse_current + amount > max_warehouse_capacity)
                {
                    show_error_and_back("Not enough warehouse space.", warehouse_current, max_warehouse_capacity);
                    break;
                }

                warehouse_current += amount;

                printf("\nOK: Delivery recorded.\n");
                printf("Warehouse: %d / %d (free: %d)\n",
                    warehouse_current,
                    max_warehouse_capacity,
                    max_warehouse_capacity - warehouse_current);

                back_to_menu_prompt();
                break;
            }

            case 2:
            {
                clear_screen();
                print_title("DELIVERY TO STORE");

                // shows error if there's no stores
                if (store_count == 0)
                {
                    show_error_and_back("No stores available.", warehouse_current, max_warehouse_capacity);
                    break;
                }

                if (warehouse_current == 0)
                {
                    show_error_and_back("Warehouse is empty.", warehouse_current, max_warehouse_capacity);
                    break;
                }

                print_stores_table(stores, store_count, store_quantities);

                int stindex = prompt_existing_store_index(stores, store_count);

                int amount = get_int("Amount to send (>0): ");
                while (amount <= 0)
                {
                    printf("ERROR: Amount must be positive.\n");
                    amount = get_int("Amount to send (>0): ");
                }

                if (amount > warehouse_current)
                {
                    show_error_and_back("Not enough items in warehouse.", warehouse_current, max_warehouse_capacity);
                    break;
                }

                warehouse_current -= amount;
                store_quantities[stindex] += amount;

                printf("\nOK: Store delivery recorded.\n");
                printf("Warehouse: %d / %d (free: %d)\n",
                    warehouse_current,
                    max_warehouse_capacity,
                    max_warehouse_capacity - warehouse_current);
                printf("Store %0*d now has: %d\n",
                    ID_PAD,
                    stores[stindex].id,
                    store_quantities[stindex]);

                back_to_menu_prompt();
                break;
            }

            case 3:
            {
                clear_screen();
                print_title("WAREHOUSE SCREEN");

                printf("Current items | Max items | Free space\n");
                printf("-------------|-----------|----------\n");
                printf("%-12d | %-9d | %-9d\n\n",
                       warehouse_current,
                       max_warehouse_capacity,
                       max_warehouse_capacity - warehouse_current);

                print_stores_table(stores, store_count, store_quantities);
                back_to_menu_prompt();
                break;
            }

            case 4:
                settings_menu(suppliers, supplier_count,
                              stores, store_count,
                              store_quantities);
                break;

            case 5:
                clear_screen();
                return 0;

            default:
                show_error_and_back("Invalid option.", warehouse_current, max_warehouse_capacity);
        }
    }
}
