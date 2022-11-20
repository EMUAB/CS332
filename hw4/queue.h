#pragma once

typedef struct _cmd_info {
	int jobid;
	char cmd[200];
} cmd_info;

typedef struct _queue
{
	int size;	 /* maximum size of the queue */
	cmd_info *buffer; /* queue buffer */
	int start;	 /* index to the start of the queue */
	int end;	 /* index to the end of the queue */
	int count;	 /* no. of elements in the queue */
} queue;

queue *queue_init(int n);
int queue_insert(queue *q, cmd_info *item);
cmd_info *queue_delete(queue *q);
void queue_display(queue *q);
void queue_destroy(queue *q);