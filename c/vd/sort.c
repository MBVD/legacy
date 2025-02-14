#include <stdio.h> 
#include <stdlib.h> 
#define SIZE 16 
 
 
typedef struct Date { 
    int day; 
    int month; 
    int year; 
} date; 
 
 
int compareDates(date d1, date d2) { 
  if (d1.year != d2.year) { 
    if (d1.year < d2.year) 
      return -1; 
    else 
      return 0; 
  }else{ 
    if (d1.month != d2.month) { 
      if (d1.month < d2.month) 
        return -1; 
      else 
        return 0; 
    } 
    else{ 
      if (d1.day < d2.day) 
        return -1; 
      else 
        if (d1.day == d2.day) 
          return 0; 
        else 
          return 1; 
    } 
  } 
   
} 
 
 
// Функция сортировки нисходящим слиянием для массива дат 
void mergeSort(date *dates, int l, int r) { 
    if (l == r) return; // границы сомкнулись 
 
    int mid = (l + r) / 2; // определяем середину последовательности 
    mergeSort(dates, l, mid); 
    mergeSort(dates, mid + 1, r); 
 
    date *tmp = (date *)malloc((r - l + 1) * sizeof(date)); // дополнительный массив 
 
    int i = l; // начало первого пути 
    int j = mid + 1; // начало второго пути 
    int c = 0; 
    // for (int step = 0; step < r - l + 1; step++) { 
    //     if ((j > r && i <= mid) || ((i <= mid) && (compareDates(dates[i], dates[j]) > 0))) { 
    //         tmp[step] = dates[i]; 
    //         i++; 
    //     } else { 
    //         tmp[step] = dates[j]; 
    //         j++; 
    //     } 
    // } 
 
    while(i <= mid && j <= r){ 
        if (compareDates(dates[i], dates[j]) == 1){ 
            tmp[c++] = dates[i++]; 
        }else{ 
            tmp[c++] = dates[j++]; 
        } 
    } 
    while(i <= mid){ 
        tmp[c++] = dates[i++]; 
    } 
    while (j<=r){ 
        tmp[c++] = dates[j++]; 
    } 
 
    for (int step = 0; step < r - l + 1; step++) { 
        dates[l + step] = tmp[step]; 
    } 
 
    free(tmp); 
} 
 
int main() { 
    date dates[SIZE]; 
 
    // Заполняем элементы массива дат 
    for (int i = 0; i < SIZE; i++) { 
        dates[i].day = rand() % 30 + 1; // Просто случайные значения для примера 
        dates[i].month = rand() % 12 + 1; 
        dates[i].year = 2000 + rand() % 100; 
         
        printf("%02d/%02d/%d ", dates[i].day, dates[i].month, dates[i].year); 
    } 
 
    mergeSort(dates, 0, SIZE - 1); // вызываем функцию сортировки 
 
    printf("\notsorterovannyi' massiv dat:\n"); 
    // Выводим отсортированный массив дат 
    for (int i = 0; i < SIZE; i++) { 
        printf("%02d/%02d/%d ", dates[i].day, dates[i].month, dates[i].year); 
    } 
}