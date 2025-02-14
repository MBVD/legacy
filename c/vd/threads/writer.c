int main() {
    key_t key;
    char* error_string;
    int work_flag = 1;
    unsigned short semaphore_value;
    struct sembuf unlock_ready_operation;
    unlock_ready_operation.sem_num = READER_READY_SEMAPHORE;
    unlock_ready_operation.sem_flg=0;
}