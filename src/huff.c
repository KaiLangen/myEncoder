/* TODO: Requires min heap and tree data structures
 *
 * Reference:
 * https://www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding/
 */


#include <stdio.h>
#include <stdlib.h>


struct element {
    char symb;
    void* left;
    void* right;
    int freq;
};


/* Initialize the heap with length, comparison function, etc. */
static void heap_init(struct min_heap* h, int max_length)
{
    h->count = 0;
    h->size = max_length;
    h->data = malloc(sizeof(struct element) * max_length);
}

/* Free all the items in the array, then free the array */ 
static void heap_destroy(struct min_heap* h)
{
    free(h->data);
    h->data = NULL;
}


/* Push new item onto the heap. Smaller items bubble up to the top*/
static void heap_push(struct min_heap* h, struct element item)
{
    /*create a new heap node*/
    int i, parent;
    struct element temp;
    if (h->count == h->size)
    {
        h->size *= 2;
        h->data = realloc(h->data, sizeof(struct element) * h->size);
        if (!h->data)
            exit(1); /* Exit if the memory allocation fails */
    }

    i = h->count;
    h->data[i] = item;
    parent = (i - 1) / 2;
    /* while the new data is bigger than it's parent, bubble up! */
    while ((i != 0) && (h->data[parent].freq > h->data[i].freq))
    {
        temp = h->data[parent];
        h->data[parent] = h->data[i];
        h->data[i] = temp;
        i = parent;
        parent = (i - 1) / 2;
    }
    /* Update the count*/
    h->count++;
}

/* Pop item from top of heap.*/
static struct element heap_pop(struct min_heap* h)
{
    int i, lfreq, rfreq, minIdx;
    struct element temp;
    struct element res;
    if (h->count == 0)
    {
        fprintf(stderr, "Unable to pop from empty heap\n");
        exit(EXIT_FAILURE);
    }
    /* store smallest element and move largest element to front of queue */
    res = h->data[0];
    h->data[0] = h->data[h->count - 1];
    /* Update the count*/
    h->count--;
    

    /* while the curr has two children, bubble down */
    i = 0;
    while (h->count > (2*i + 2)) 
    {
				lfreq = h->data[2*i + 1].freq;
				rfreq = h->data[2*i + 2].freq;
				minIdx = lfreq > rfreq ? 2*i + 1 : 2*i + 2;
				if (h->data[i] < h->data[minIdx])
				{
						temp = h->data[minIdx];
						h->data[minIdx] = h->data[i];
						h->data[i] = temp;
						i = minIdx;
				}
    }

    /* one final check to see if curr has a left child that's smaller */
    if (h->count > (2*i + 1) & h->data[i].freq > h->data[2*i + 1].freq)
		{
				temp = h->data[minIdx];
				h->data[minIdx] = h->data[i];
				h->data[i] = temp;
				i = minIdx;
		}
    return res;
}


/*
 * For every block code, iterate over every symbol in the code, and create 
 * an "element" for every unique symbol.
 */
int huffman(struct element, char **code_blocks, int code_sizes[]; int nblocks)
{
    int char_count[256] = {0};
    struct min_heap h;
    struct element temp1;
    struct element temp2;
    struct element* ep;

    temp1.left = NULL;
    temp1.left = NULL;
    temp1.freq = 0;

    /* initialize the heap */
    init_heap(&h, 256);

    /* initialize the char_counts */
    for (int i = 0; i < nblocks; ++i)
        for (int i = 0; i < code_sizes[i]; ++i)
            char_count[(unsigned char)code_block[i]]++;

    /* initialize the char_vals and push onto heap */
    for (unsigned char c = 0; c < 256; ++c)
    {
        if(char_count[c] > 0)
        {
            temp1.symb = c;
            temp1.freq = char_count[c];
            heap_push(&h, temp1);
        }
    }

    /* coalesce the elements into a tree */
    while (h.count > 1)
    {
        ep = (struct element*)malloc(sizeof(struct  element));
        *ep = heap_pop(&h);
        temp2 = heap_pop(&h);
        if (temp1.right && temp2.right)

        else if (temp1.right)
        temp.right = ep;
        heap_push(&h, temp);
    }
}
