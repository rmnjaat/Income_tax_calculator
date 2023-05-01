#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct taxpayer
{
    char name[50];
    int Pancard_Number;
    int Mobileno;
    float income;
    float tax;
};

void add_record(FILE *file)
{
    struct taxpayer record;
    printf("Enter name: ");
    scanf("%s", record.name);
    printf("Enter 5-digit Pancard_number: ");
    scanf("%d", &record.Pancard_Number);
    printf("Enter Mobileno: ");
    scanf("%d", &record.Mobileno);
    printf("Enter income: ");
    scanf("%f", &record.income);

    if (record.income <= 25000)
    {
        record.tax = 0;
    }
    else if (record.income > 25000 && record.income <= 50000)
    {
        record.tax = (record.income - 25000) * 0.05;
    }
    else if (record.income > 50000 && record.income <= 75000)
    {
        record.tax = 1250 + ((record.income - 50000) * 0.2);
    }
    else if (record.income > 75000 && record.income <= 100000)
    {
        record.tax = 5520 + ((record.income - 75000) * 0.3);
    }
    else
    {
        record.tax = 11250 + ((record.income - 100000) * 0.4);
    }

    // record.tax = record.income * 0.1; // 10% tax rate

    fwrite(&record, sizeof(struct taxpayer), 1, file);
    printf("Record added successfully.\n");
}

void show_allrecord(FILE *file)
{
    // rewind(file);
    struct taxpayer record;
    fseek(file, 0, SEEK_SET);
    printf("%-20s %-20s %-20s %-20s  \n", "Name", "PancardNo.", "Income", "TaxPaid");
    while (fread(&record, sizeof(struct taxpayer), 1, file))   //while(1)
    {
        printf("%-20s %-19d Rs.%-19.2f Rs.%-17.2f  \n", record.name, record.Pancard_Number, record.income, record.tax);
    }
}

void search_record(FILE *file)
{
    // char name[50];
    int search;
    struct taxpayer record;
    printf("Enter pancard number to search: ");
    scanf("%d", &search);
    fseek(file, 0, SEEK_SET);
    while (fread(&record, sizeof(struct taxpayer), 1, file))
    {
        if (record.Pancard_Number == search)
        {
            printf("%-20s %-20s %-20s %-20s  \n", "Name", "PancardNo.", "Income", "TaxPaid");

            printf("%-20s %-19d Rs.%-19.2f Rs.%-17.2f  \n", record.name, record.Pancard_Number, record.income, record.tax);

            return;
        }
    }
    printf("Record not found.\n");
}

void *edit_record(FILE *file)
{
    int to_search;
    printf("Enter the pancard number of the taxpayer  TO UPDATE : ");
    scanf("%d", &to_search);
    fseek(file, 0, SEEK_SET);

    struct taxpayer record;
    int found = 0;

    // Create a temporary file
    FILE *temp = fopen("temp.txt", "wb");

    // Read each record from the file and write to the temporary file except for the one to be deleted
    while (fread(&record, sizeof(record), 1, file) == 1)
    {
        if (record.Pancard_Number == to_search)
        {
            found = 1;
            printf("Enter new income: ");
            scanf("%f", &record.income);

            if (record.income <= 25000)
            {
                record.tax = 0;
            }
            else if (record.income > 25000 && record.income <= 50000)
            {
                record.tax = (record.income - 25000) * 0.05;
            }
            else if (record.income > 50000 && record.income <= 75000)
            {
                record.tax = 1250 + ((record.income - 50000) * 0.2);
            }
            else if (record.income > 75000 && record.income <= 100000)
            {
                record.tax = 5520 + ((record.income - 75000) * 0.3);
            }
            else
            {
                record.tax = 11250 + ((record.income - 100000) * 0.4);
            }
            fwrite(&record, sizeof(record), 1, temp);
        }
        else
        {
            fwrite(&record, sizeof(record), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);

    if (found)
    {
        // Delete the original file and rename the temporary file to the original file name
        remove("taxpayers.txt");
        rename("temp.txt", "taxpayers.txt");
        // printf("Taxpayer with pan number %d has been deleted.\n", to_search);
        printf("Tax payer has been updated ");
    }
    else
    {
        printf("Taxpayer with pan number %d not found.\n", to_search);
        remove("temp.txt");
    }

    // Open the original file for appending
    file = fopen("taxpayers.txt", "ab+"); // ab+
    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    return file;
}


FILE *delete_record(FILE *file)
{

    int to_search;
    printf("Enter the pancard number of the taxpayer to delete: ");
    scanf("%d", &to_search);
    fseek(file, 0, SEEK_SET);

    struct taxpayer record;
    int found = 0;

    // Create a temporary file
    FILE *temp = fopen("temp.txt", "wb");

    // Read each record from the file and write to the temporary file except for the one to be deleted
    while (fread(&record, sizeof(record), 1, file) == 1)
    {
        if (record.Pancard_Number == to_search)
        {
            found = 1;
        }
        else
        {
            fwrite(&record, sizeof(record), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);

    if (found)
    {
        // Delete the original file and rename the temporary file to the original file name
        remove("taxpayers.txt");
        rename("temp.txt", "taxpayers.txt");
        printf("Taxpayer with pan number %d has been deleted.\n", to_search);
    }
    else
    {
        printf("Taxpayer with pan number %d not found.\n", to_search);
        remove("temp.txt");
    }

    // Open the original file for appending
    file = fopen("taxpayers.txt", "ab+"); // ab+
    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    return file;
}



int main()
{
    FILE *file = fopen("taxpayers.txt", "ab+"); // ab+
    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }

    int choice;
    do
    {
        printf("\n1. Add New Record\n");
        printf("2. List All Tax Payer along with Income Tax to be paid\n");
        printf("3. Search\n");
        printf("4. Edit\n");
        printf("5. Delete Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add_record(file);
            break;
        case 2:
            show_allrecord(file);
            break;
        case 3:
            search_record(file);
            break;
        case 4:
            edit_record(file);
            break;
        case 5:
            delete_record(file);
            break;
        case 6:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }

    } while (choice != 6);
    fclose(file);
    return 0;
}
