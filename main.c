#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TCoada.h"
#include "TStiva.h"
#include <inttypes.h>

int VidS(void *a);

typedef struct thread
{
    int ID_thread; //unsigned int ID_thread;
} Thread, *AThread;

typedef struct task
{
    int ID_task; //unsigned short ID_task;
    Thread *thread;
    int prioritate; //int8_t prioritate;
    int time; //unsigned int time;
} Task, *ATask;

void afiStiva(TStiva a)
{
    TStiva *aux = &a;
    while(aux->vf!=NULL){
        Thread thread;
        Pop(aux,(void*) &thread);
        printf("%d\n", thread.ID_thread);

    }
}

void get_thread(int id, ASt thread_pool, FILE *foutput)
{
    ASt aux = InitS(sizeof(Thread));
    Thread thread;
    while(!VidS(thread_pool)){
        Pop(thread_pool, &thread);
        if(thread.ID_thread == id){
            fprintf(foutput, "Thread %d is idle.\n", thread.ID_thread);
        }
        Push(aux, &thread);
    }
    while(!VidS(aux)){
        Pop(aux, &thread);
        Push(thread_pool, &thread);
    }

}

void get_task(int id, AQ waiting, AQ running, AQ finished, FILE *foutput)
{
    int contor = 0;
    AQ aux = InitQ(waiting->dime);
    Task task;
    if(contor == 0){
        while(!VidQ(waiting)){
            ExtrQ(waiting,&task);
            if(task.ID_task == id){
                fprintf(foutput, "Task %d is waiting (remaining_time = %d).\n", task.ID_task, task.time);
                contor+=1;
            }
            IntrQ(aux, &task);
        }
        ConcatQ(waiting, aux); 
    }
    if(contor == 0){
        while(!VidQ(running)){
            ExtrQ(running, &task);
            if(task.ID_task == id){
                fprintf(foutput, "Task %d is running (remaining_time = %d).\n", task.ID_task, task.time);
                contor+=1;        
            }
            IntrQ(aux, &task);
            
        }
        ConcatQ(running, aux);
    }

    if(contor == 0){
        while(!VidQ(finished)){
            ExtrQ(running, &task);
            if(task.ID_task == id){
                fprintf(foutput, "Task %d is running (remaining_time = %d).\n", task.ID_task, task.time);
                return;
                contor+=1;
            }
            IntrQ(aux, &task);
            
        }
        ConcatQ(finished, aux);
    }
    if(contor == 0){
        fprintf(foutput, "Task %d not found.\n", id);      
    }
}

void print_waiting(AQ a, FILE *foutput)
{
    Task task;
    AQ aux = InitQ(a->dime);
    fprintf(foutput, "====== Waiting queue =======\n[");

    if (!VidQ(a)){
        ExtrQ(a,(void*) &task);
        fprintf(foutput, "(%d: priority = %d, remaining_time = %d)", task.ID_task, task.prioritate, task.time);
        IntrQ(aux, &task);
    }
    while(!VidQ(a)){
        ExtrQ(a,(void*) &task);
        fprintf(foutput, ",\n(%d: priority = %d, remaining_time = %d)", task.ID_task, task.prioritate, task.time);
        IntrQ(aux, &task);
    }
    fprintf(foutput, "]\n");

    ConcatQ(a, aux);        
}

void print_finished(AQ a, FILE *foutput)
{
    Task task;
    AQ aux = InitQ(a->dime);
    fprintf(foutput, "====== Finished queue =======\n[");
    int exec_time = 0; //placeholder pt ce o sa dea functia de running pls no hate
    if (!VidQ(a)){
        ExtrQ(a,(void*) &task);
        fprintf(foutput, "(%d: priority = %d, executed_time = %d)", task.ID_task, task.prioritate, exec_time);
        IntrQ(aux, &task);
    }
    while(!VidQ(a)){
        ExtrQ(a,(void*) &task);
        fprintf(foutput, ",\n(%d: priority = %d, executed_time = %d)", task.ID_task, task.prioritate, exec_time);
        IntrQ(aux, &task);
    }
    fprintf(foutput, "]\n");

    ConcatQ(a, aux);        
}void print_running(AQ a, FILE *foutput)
{
    Task task;
    AQ aux = InitQ(a->dime);
    fprintf(foutput, "====== Running in parallel =======\n[");
    if (!VidQ(a)){
        ExtrQ(a,(void*) &task);
        fprintf(foutput, ",\n(%d: priority = %d, remaining_time = ", task.ID_task, task.prioritate);
        fprintf(foutput, "%d, running_thread =%d)", task.time, task.thread->ID_thread);
        IntrQ(aux, &task);
    }
    while(!VidQ(a)){
        ExtrQ(a,(void*) &task);
        fprintf(foutput, ",\n(%d: priority = %d, remaining_time = ", task.ID_task, task.prioritate);
        fprintf(foutput, "%d, running_thread =%d)", task.time, task.thread->ID_thread);
        IntrQ(aux, &task);
    }
    fprintf(foutput, "]\n");

    ConcatQ(a, aux);        
}
    

ASt InitPool(int N)
{
    void* ThreadPool = InitS(sizeof(Thread));
    for(int i=N-1; i>=0; i--){
        AThread thread = (AThread)malloc(sizeof(Thread));
        thread->ID_thread = i;
        Push(ThreadPool, thread);
    }
    return ThreadPool;
}

void add_tasks(AQ q, ATask task, FILE* foutput)
{
    if(VidQ(q)){
        IntrQ(q, task);
        fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
        return;
    }
    AQ WaitingQ_aux = InitQ(sizeof(Task));
    ATask task_aux = malloc(sizeof(Task));
    while(!VidQ(q)){
        ExtrQ(q, task_aux);
        
        if(task->prioritate > task_aux->prioritate){
            IntrQ(WaitingQ_aux, task);
            IntrQ(WaitingQ_aux, task_aux);
            ConcatQ(WaitingQ_aux, q);
            ConcatQ(q, WaitingQ_aux);
            fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
            free(task_aux);
            return;

        }
        else if(task->prioritate == task_aux->prioritate){
            if(task->time < task_aux->time){
                IntrQ(WaitingQ_aux, task);
                IntrQ(WaitingQ_aux, task_aux);
                ConcatQ(WaitingQ_aux, q);
                ConcatQ(q, WaitingQ_aux);
                fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                free(task_aux);
                return;
            }
            else if(task->time == task_aux->time){
                if(task->ID_task < task_aux->ID_task){
                    IntrQ(WaitingQ_aux, task);
                    IntrQ(WaitingQ_aux, task_aux);
                    ConcatQ(WaitingQ_aux, q);
                    ConcatQ(q, WaitingQ_aux);
                    fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                    free(task_aux);
                    return;
                }
                else if(task->ID_task > task_aux->ID_task){
                    if(VidQ(q)){
                        IntrQ(WaitingQ_aux, task_aux);
                        IntrQ(WaitingQ_aux, task);
                        ConcatQ(q, WaitingQ_aux);
                        fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                        free(task_aux);
                        return;
                    }
                    else {
                        while(task->prioritate == task_aux->prioritate && task->time == task_aux->time && VidQ(q) != 1){
                            IntrQ(WaitingQ_aux, task_aux);
                            ExtrQ(q, task_aux);  
                        }
                        if(VidQ(q) == 1 && task->prioritate == task_aux->prioritate && task->time == task_aux->time){
                            IntrQ(WaitingQ_aux, task_aux);
                            IntrQ(WaitingQ_aux, task);
                            ConcatQ(WaitingQ_aux, q);
                            ConcatQ(q, WaitingQ_aux);
                            fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                            free(task_aux);
                            return;
                        }
                        else if(VidQ(q) == 1 && (task->prioritate != task_aux->prioritate || task->time != task_aux->time)){
                            IntrQ(WaitingQ_aux, task);
                            IntrQ(WaitingQ_aux, task_aux);
                            ConcatQ(WaitingQ_aux, q);
                            ConcatQ(q, WaitingQ_aux);
                            fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                            free(task_aux);
                            return;
                        }
                        else{
                            IntrQ(WaitingQ_aux, task);
                            IntrQ(WaitingQ_aux, task_aux);
                            ConcatQ(WaitingQ_aux, q);
                            ConcatQ(q, WaitingQ_aux);
                            fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                            free(task_aux);
                            return;
                        }
                        
                    }
                }
            }
            else if(task->time > task_aux->time){
                IntrQ(WaitingQ_aux, task_aux);
                if(VidQ(q)){
                    IntrQ(WaitingQ_aux, task);
                    ConcatQ(q, WaitingQ_aux);
                    fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                    free(task_aux);
                    return;
                }
                else
                    continue;
            }
        }
        else if(task->prioritate < task_aux->prioritate){
            IntrQ(WaitingQ_aux, task_aux);
            if(VidQ(q)){
                IntrQ(WaitingQ_aux, task);
                ConcatQ(q, WaitingQ_aux);
                fprintf(foutput, "Task created successfully : ID %d.\n", task->ID_task);
                free(task_aux);
                return;
            }
            else
                continue;
        }
    }      
}


int main(int argc, char **argv)
{
    FILE *f, *foutput;
    f = fopen(argv[1], "rt");
    foutput = fopen(argv[2], "wt");
    if (f == NULL)
        return 0;
    if(foutput == NULL)
        return 0;
    char *number_of_tasks, *exec_time_in_ms, *priority_char;
    char *inst;
    char *print_type = malloc(10* sizeof(char));
    char *line = malloc(256 * sizeof(char));
    int tasks, time, priority;
    unsigned int cuanta,cores,N;
    fscanf(f,"%i%i",&cuanta,&cores);
    N = cores * 2;
    ASt s = InitPool(N);
    AQ WaitingQ = InitQ(sizeof(Task));
    AQ RunningQ = InitQ(sizeof(Task));
    AQ FinishedQ = InitQ(sizeof(Task));
    int8_t *vector_id;
    vector_id = calloc(32767, sizeof(int8_t));
    char *id_char;
    int id;
    while (fscanf(f, "%s", line) == 1)
    {
        if(!strcmp(line, "add_tasks")) {
            inst = malloc(256*sizeof(char));
            fgets(inst,256, f);
            number_of_tasks = strtok(inst, " \n");
            exec_time_in_ms = strtok(NULL, " \n");
            priority_char = strtok(NULL, " \n");
            tasks = atoi(number_of_tasks);
            time = atoi(exec_time_in_ms);
            priority = atoi(priority_char);

            for(int i = 0; i < tasks; i++){
                ATask task = (ATask)malloc(sizeof(Task));
                task->prioritate = priority;
                task->time = time;
                for(int j = 1; j <= 32767; j++){
                    if(vector_id[j]==0){
                        vector_id[j]=1;
                        task->ID_task = j;
                        break;
                    }
                }
                task->thread = malloc(sizeof(Thread));
                add_tasks(WaitingQ, task, foutput);
                //free(inst);
            }
        }
        else if(!strcmp(line, "print")){
            inst = malloc(256*sizeof(char));
            fgets(inst,256, f);
            print_type = strtok(inst, " \n");
            if(!strcmp(print_type, "waiting")){
                print_waiting(WaitingQ, foutput);
            }
            else if(!strcmp(print_type, "running")){
                print_running(RunningQ, foutput);
            }
            else if(!strcmp(print_type, "finished")){
                print_finished(FinishedQ, foutput);
            }
            //free(inst);
        }
        else if(!strcmp(line, "get_task")){
            inst = malloc(256*sizeof(char));
            fgets(inst,256, f);
            id_char = strtok(inst, " \n");
            id = atoi(id_char);
            get_task(id, WaitingQ, RunningQ, FinishedQ, foutput);
            //free(inst);
            
        }
        else if(!strcmp(line, "get_thread")){
            inst = malloc(256*sizeof(char));
            fgets(inst,256, f);
            id_char = strtok(inst, " \n");
            id = atoi(id_char);
            get_thread(id, s, foutput);
            //free(inst);
        }
            
    }
    free(inst);
    free(line);
    fclose(f);
    fclose(foutput);
    return 0;
}