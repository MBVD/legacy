#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Date {
    int day;
    int month;
    int year;
};

struct Date inputDate() {
    struct Date date;
    printf("Ð’Ð²ÐµÐ´Ð¸Ñ‚Ðµ Ð´Ð°Ñ‚Ñƒ Ð² Ñ„Ð¾Ñ€Ð¼Ð°Ñ‚Ðµ Ð”Ð”.ÐœÐœ.Ð“Ð“Ð“Ð“: ");
    scanf("%d.%d.%d", &date.day, &date.month, &date.year);
    return date;
}

void swap(struct Date* first, struct Date* second){
    struct Date temp = *second;
    *second = *first;
    *first = temp;
}


int is_place(int j, struct Date temp, int gap, struct Date arr[])
{
    return (arr[j - gap].year > temp.year 
    || (arr[j - gap].year == temp.year && arr[j - gap].month > temp.month) 
    || (arr[j - gap].year == temp.year && arr[j - gap].month == temp.month && arr[j - gap].day > temp.day));
}
 

int ShellSort(struct Date arr[], int n) {
    int count = 0;
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            struct Date temp = arr[i];
            int j;
            for (j = i; j >= gap && is_place(j, temp, gap, arr); j -= gap) {
                arr[j] = arr[j - gap];
                count++;
            }
            arr[j] = temp;
        }
    }
    return count;
}

struct Date generateRandomDate() {
    struct Date temp;
    int year = rand() %9999 + 1;
    temp.year = year;

    int month = rand() % 12 + 1;
    temp.month = month;
    int maxDay;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            maxDay = 29;
        } else {
            maxDay = 28;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;
    } else {
        maxDay = 31;
    }

    int day = rand() % maxDay + 1;
    temp.day = day;
    return temp;
}

struct Date* generateRandomDates(int n, struct Date dates[n]){
    for (int i = 0; i < n; i++){
        dates[i] = generateRandomDate();
    }
    return dates;
}

void printDates(struct Date arr[], int n) {
    printf("ÐžÑ‚ÑÐ¾Ñ€Ñ‚Ð¸Ñ€Ð¾Ð²Ð°Ð½Ð½Ñ‹Ðµ Ð´Ð°Ñ‚Ñ‹:\n");
    for (int i = 0; i < n; i++) {
        printf("%02d.%02d.%04d\n", arr[i].day, arr[i].month, arr[i].year);
    }
}

int main() {
    srand(time(NULL));
    int size;
    printf("Ð’Ð²ÐµÐ´Ð¸Ñ‚Ðµ ÐºÐ¾Ð»Ð¸Ñ‡ÐµÑÑ‚Ð²Ð¾ Ð´Ð°Ñ‚: ");
    scanf("%d", &size);

    struct Date *dates = (struct Date *)malloc(size * sizeof(struct Date));

    dates = generateRandomDates(size, dates);

    int swaps = ShellSort(dates, size);

    printDates(dates, size);

    printf("ÐšÐ¾Ð»Ð¸Ñ‡ÐµÑÑ‚Ð²Ð¾ Ð¿ÐµÑ€ÐµÑÑ‚Ð°Ð½Ð¾Ð²Ð¾Ðº: %d\n", swaps);

    free(dates);
    return 0;
}


/*Ð¿ÐµÑ€Ð²Ñ‹Ð¹ Ð·Ð°Ð¿ÑƒÑÐº 1000 Ð´Ð°Ñ‚ - 8139
Ð²Ñ‚Ð¾Ñ€Ð¾Ð¹ Ð·Ð°Ð¿ÑƒÑÐº - 7797
Ñ‚Ñ€ÐµÑ‚Ð¸Ð¹ Ð·Ð°Ð¿ÑƒÑÐº - 8222
Ñ‡ÐµÑ‚Ð²ÐµÑ€Ñ‚Ñ‹Ð¹ Ð·Ð°Ð¿ÑƒÑÐº - 7110
Ð¿ÑÑ‚Ñ‹Ð¹ Ð·Ð°Ð¿ÑƒÑÐº - 7644
ÑˆÐµÑÑ‚Ð¾Ð¹ Ð·Ð°Ð¿ÑƒÑÐº - 8141
ÑÐµÐ´ÑŒÐ¼Ð¾Ð¹ Ð·Ð°Ð¿ÑƒÑÐº - 7755
Ð²Ð¾ÑÑŒÐ¼Ð¾Ð¹ - 8342
9 - 7894
10 - 7580
ÑÑ€ÐµÐ´Ð½ÐµÐµ Ð·Ð½Ð°Ñ‡ÐµÐ½Ð¸Ðµ - 7862,4
*/