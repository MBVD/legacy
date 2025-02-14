#include <stdio.h>
#include <stdlib.h>
struct Time{
    int hours, minutes, seconds;
};

struct Time init(hours, minutes, seconds){
    struct Time time;
    time.hours = hours;
    time.minutes = minutes;
    time.seconds = seconds;
    return time;
}  

struct Time sum_time(struct Time time1, struct Time time2){
    long long seconds = time1.hours * 3600 + time1.minutes * 60 + time1.seconds + time2.hours * 3600 + time2.minutes * 60 + time2.seconds;
    int hours = seconds / 3600;
    seconds -= hours * 3600;
    int minutes = seconds / 60;
    seconds -= minutes * 60;
    struct Time time;
    time.hours = hours;
    time.minutes = minutes;
    time.seconds = seconds;
    return time;
}

int main(){
    int hours1, minutes1, seconds1, hours2, minutes2, seconds2;
    printf("hours1, minutes1, seconds1 \n");
    scanf("%d%d%d", &hours1, &minutes1, &seconds1);
    printf("hours2, minutes2, seconds2 \n");
    scanf("%d%d%d", &hours2, &minutes2, &seconds2);
    struct Time time1, time2;
    time1 = init(hours1, minutes1, seconds1);
    time2 = init(hours2, minutes2, seconds2);
    struct Time time_ans;
    time_ans = sum_time(time1, time2);
    printf("hours: %d minutes: %d seconds: %d \n", time_ans.hours, time_ans.minutes, time_ans.seconds);
    // printf("%d", date.time.hours);

}
