#include <stdio.h>
#include <stdlib.h>
int cnt = 0;

typedef struct Date {
    int year;
    int month;
    int day;
} date;

int comp_date(date date1, date date2){
    if (date1.year == date2.year && date1.month == date2.month && date1.day == date2.day){
        return 0;
    }
    if (date1.year > date2.year){
        return 1;
    }
    if (date1.year == date2.year && date1.month > date2.month){
        return 1;
    }
    if (date1.year == date2.year && date1.month == date2.month && date1.day > date2.day){
        return 1;
    }
    return -1;
}

date* sort(date* a, int n){
    // [1, 2, 3] -> [1, 2], [3]
    date* pol1 = calloc (n/2 + n%2, sizeof(date)); date* pol2 = calloc (n/2, sizeof(date));
    date* ans = calloc(n, sizeof(date));
    if (n == 1){ 
        return a;
    }
    int j = 0;
    for (int i = 0; i<n; i++){
        if (i < n/2 + n%2){
            pol1[i] = a[i];
        }else{
            pol2[j++] = a[i];
        }
    }
    pol1 = sort(pol1, n/2 + n%2);
    pol2 = sort(pol2, n/2);
    j = 0;
    int i = 0, c = 0;
    while (i != n/2 + n%2 && j != n/2){
        if (comp_date(pol1[i], pol2[j]) == 1){
            ans[c++] = pol2[j++];
            cnt++;
        }else{
            ans[c++] = pol1[i++];
            cnt++;
        }
    }
    // [1, 2, 3], [];
    while(i != n/2 + n%2){
        ans[c++] = pol1[i++];
        cnt++;
    }
    // [] [1, 2, 3]
    while(j != n/2){
        ans[c++] = pol2[j++];
        cnt++;
    }
    free(pol1);
    free(pol2);
    return ans;
}

int main(){
    int n; scanf("%d", &n);
    date* a = calloc(n, sizeof(date));
    for (int i = 0; i<n; i++){
        date d;
        // scanf("%d %d %d", &d.year, &d.month, &d.day);
        d.year = rand() % 1000;
        d.month = rand() % 12 + 1;
        d.day = rand() % 30 + 1;
        a[i] = d;
    }
    date* ans = sort(a, n);
    printf("%d", cnt);
}