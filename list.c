#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

list_t *list_alloc() {
    list_t *l = (list_t *)malloc(sizeof(list_t));
    if (l != NULL) {
        l->head = NULL;
    }
    return l;
}

void list_free(list_t *l) {
    if (l == NULL) return;
    node_t *curr = l->head;
    while (curr != NULL) {
        node_t *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l);
}

void list_print(list_t *l) {
    if (l == NULL) return;
    node_t *curr = l->head;
    while (curr != NULL) {
        printf("%d->", curr->value);
        curr = curr->next;
    }
    printf("NULL\n");
}

char *listToString(list_t *l) {
    char *buf = (char *)malloc(1024);
    if (buf == NULL) return NULL;
    buf[0] = '\0';

    if (l == NULL || l->head == NULL) {
        snprintf(buf, 1024, "NULL");
        return buf;
    }

    node_t *curr = l->head;
    char temp[64];
    while (curr != NULL) {
        snprintf(temp, sizeof(temp), "%d->", curr->value);
        strncat(buf, temp, 1024 - strlen(buf) - 1);
        curr = curr->next;
    }
    strncat(buf, "NULL", 1024 - strlen(buf) - 1);
    return buf;
}

int list_length(list_t *l) {
    if (l == NULL) return 0;
    int count = 0;
    node_t *curr = l->head;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

void list_add_to_front(list_t *l, elem val) {
    if (l == NULL) return;
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) return;
    new_node->value = val;
    new_node->next = l->head;
    l->head = new_node;
}

void list_add_to_back(list_t *l, elem val) {
    if (l == NULL) return;
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) return;
    new_node->value = val;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
        return;
    }

    node_t *curr = l->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
}

void list_add_at_index(list_t *l, elem val, int index) {
    if (l == NULL || index < 1) return;
    if (index == 1) {
        list_add_to_front(l, val);
        return;
    }

    node_t *curr = l->head;
    int pos = 1;
    while (curr != NULL && pos < index - 1) {
        curr = curr->next;
        pos++;
    }

    if (curr == NULL) return;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) return;
    new_node->value = val;
    new_node->next = curr->next;
    curr->next = new_node;
}

elem list_remove_from_front(list_t *l) {
    if (l == NULL || l->head == NULL) return -1;
    node_t *temp = l->head;
    elem val = temp->value;
    l->head = temp->next;
    free(temp);
    return val;
}

elem list_remove_from_back(list_t *l) {
    if (l == NULL || l->head == NULL) return -1;
    if (l->head->next == NULL) {
        elem val = l->head->value;
        free(l->head);
        l->head = NULL;
        return val;
    }

    node_t *curr = l->head;
    while (curr->next->next != NULL) {
        curr = curr->next;
    }
    elem val = curr->next->value;
    free(curr->next);
    curr->next = NULL;
    return val;
}

elem list_remove_at_index(list_t *l, int index) {
    if (l == NULL || l->head == NULL || index < 1) return -1;
    if (index == 1) {
        return list_remove_from_front(l);
    }

    node_t *curr = l->head;
    int pos = 1;
    while (curr != NULL && pos < index - 1) {
        curr = curr->next;
        pos++;
    }

    if (curr->next == NULL) return -1;

    node_t *temp = curr->next;
    elem val = temp->value;
    curr->next = temp->next;
    free(temp);
    return val;
}

bool list_is_in(list_t *l, elem val) {
    if (l == NULL) return false;
    node_t *curr = l->head;
    while (curr != NULL) {
        if (curr->value == val) return true;
        curr = curr->next;
    }
    return false;
}

elem list_get_elem_at(list_t *l, int index) {
    if (l == NULL || index < 1) return -1;
    node_t *curr = l->head;
    int pos = 1;
    while (curr != NULL) {
        if (pos == index) return curr->value;
        curr = curr->next;
        pos++;
    }
    return -1;
}

int list_get_index_of(list_t *l, elem val) {
    if (l == NULL) return -1;
    node_t *curr = l->head;
    int pos = 1;
    while (curr != NULL) {
        if (curr->value == val) return pos;
        curr = curr->next;
    }
    return -1;
}