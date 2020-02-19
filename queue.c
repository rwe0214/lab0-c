#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    for (list_ele_t *del = q->head; q->head; del = q->head) {
        q->head = del->next;
        free(del->value);
        free(del);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;

    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->value = malloc(strlen(s) * sizeof(char) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    newh->next = q->head;
    strncpy(newh->value, s, strlen(s) * sizeof(char));
    (newh->value)[strlen(s) * sizeof(char)] = '\0';
    q->head = newh;
    if (q->size++ == 0)
        q->tail = newh;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = malloc(strlen(s) * sizeof(char) + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }
    newt->next = NULL;
    strncpy(newt->value, s, strlen(s) * sizeof(char) + 1);
    (newt->value)[strlen(s) * sizeof(char)] = '\0';

    q->tail->next = newt;
    q->tail = newt;
    if (q->size++ == 0)
        q->head = newt;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    if (!!sp) {
        strncpy(sp, tmp->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    q->size--;
    free(tmp->value);
    free(tmp);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return !!q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    list_ele_t *iter, *nexti, *prei = NULL;
    q->tail = q->head;
    for (iter = q->head; iter->next; iter = nexti) {
        nexti = iter->next;
        iter->next = prei;
        prei = iter;
    }
    iter->next = prei;
    q->head = iter;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || q->size <= 1)
        return;
    q->head = sort(q->head);
}

list_ele_t *sort(list_ele_t *start)
{
    if (!start || !start->next)
        return start;

    list_ele_t *left = start;
    list_ele_t *right = left->next;

    left->next = NULL;

    right = sort(right);

    for (list_ele_t *merge = NULL; left || right;) {
        if (!right || (left && strcmp(left->value, right->value) < 0)) {
            if (!merge) {
                start = merge = left;
            } else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        } else {
            if (!merge) {
                start = merge = right;
            } else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        }
    }
    return start;
}
