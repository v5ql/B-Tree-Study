// ��Ʈ�� Ž��, ����, ���� ���α׷�
// �Է�: 2 ���� ȸ��� ���� - Com_names1.txt,  Com_names2.txt
//       (����: ȸ����� �߰��� space ���ڸ� ������ �� �� ����.)
// ���� �� ���� ���� ��� ȸ����� �� ���ڵ�� �Ͽ� �ְ�,
//  �� ����  ��ɹ� ���� ������ ������.

#define	_CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXK 2				// MAXK��  2d   ��
#define HALFK MAXK/2        // d (capacity order) �̴�.
#define MAX 100				// ���� �ִ� ���Ҽ�.
#define false 0
#define true 1

typedef struct element {		// ���ڵ� ����.  ȸ���� �� ���̷� ������
	char name[100];				// ȸ���
	int nleng;					// ȸ��� ����
}ele;

typedef struct node* nodeptr;	// *nodeptr�� node�� ���¸� ���� �������̴�. 
typedef struct node {           // �Ϲ� ��� 
	int fill_cnt;
	ele rec[MAXK];
	nodeptr ptr[MAXK + 1];
}node;
typedef struct big_node {       // �Ϲݳ�庸�� ���ڵ�� �����Ͱ� �ϳ��� �� ū ���� [�պ��� ���] 
	nodeptr ptr[MAXK + 2];
	ele rec[MAXK + 1];
}big_node;
typedef struct two_Bn {
	nodeptr ptr[2 * MAXK + 1];
	ele rec[2 * MAXK];
} two_Bnode;

nodeptr root = NULL;		// root�� ���� ������ ����Ѵ�. 
//nodeptr temp;
nodeptr stack[MAX];			//stack�� max���� 100
int top = -1;

void push(nodeptr node);
nodeptr pop();

void insert_btree();					// file ��ü�� ���� �� �Լ��� insert_arec�� ȣ���Ѵ�. 
int insert_arec(ele in_rec);			// ���ڵ带 �ϳ� �����Ѵ�. 
nodeptr retrieve(char*, int*);			// Ű���� �־� �˻��Ѵ�. [���̿� �Բ� ���] 
int seq_scan_btree(nodeptr curr);		// ��ü ���ڵ���� ����Ѵ�. 
int B_tree_deletion(char*);				// Function to delete a record with a given key.

int total_height = 0;					// ��ü ���� 
int main(void) {

	char input, line[200], * res_gets;
	ele a_insert;
	char name_s[100];
	int num, lleng;
	int i, k, r, find;
	nodeptr tp;

	insert_btree();

	// ��ɹ� ���� ����.
	while (1) {
		fflush(stdin);	// buffer clear 
		printf("\n����� �����ÿ�\n");
		printf("insert: i �̸�  / delete : d �̸�  /  retrieve : r �̸�  /  ��ü���: p   /  ����: e  >> ");
		res_gets = gets(line);
		if (!res_gets)
			break;		// no input.

		lleng = strlen(line);
		if (lleng <= 0)
			continue;	// empty line input. try again.

		i = 1;
		if (line[0] == 'E' || line[0] == 'e') {		// exit program
			printf("�������� ������!\n\n");   return 0;
		}

		else if (line[0] == 'I' || line[0] == 'i') { // ���ڵ� �Ѱ� ���� ���.

			// ���� ȸ����� ������.
			k = 0;
			while (line[i] == ' ')	i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;

			strcpy(a_insert.name, name_s);
			a_insert.nleng = strlen(name_s);

			top = -1;
			r = insert_arec(a_insert);	// ���ڵ� �ϳ��� �����ϴ� �Լ��� ȣ�� 
			if (r == 0)
				printf("���� ����.\n", r);
			else
				printf("���� ����.\n", r);

		}
		else if (line[0] == 'D' || line[0] == 'd') {  // ���� ��� ����
			k = 0;
			while (line[i] == ' ')	i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;

			r = B_tree_deletion(name_s);
			if (r == 0)
				printf("������û ���� Ű�� ���ڵ尡 �������� ����.\n", r);
			else
				printf("���� ����.\n");

		}
		else if (line[0] == 'R' || line[0] == 'r') {	// Ž�� ��� ����
			k = 0;
			while (line[i] == ' ')	i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;
			tp = retrieve(name_s, &i);
			if (tp)
				printf("Ž������.  Name: %s,  ����: %d\n", tp->rec[i].name, tp->rec[i].nleng);
			else
				printf("Ž����û ���� Ű�� ���� ���ڵ尡 �������� ����.\n");

		}
		else if (line[0] == 'P' || line[0] == 'p') // ����Ʈ ��� ����. ��� ���ڵ带 Ű ������ ���.
			seq_scan_btree(root);
		else
			printf("input error \n");
	}	//while
}	//main

/**************** stack ���� �Լ��� ********************/
void push(nodeptr node) {
	if (top > MAX) /// >>>  MAX-1 �̻�( >= ) �̾�� ���� �ʳ�?
	{
		printf("stack is full\n");
		return;
	}
	top++;
	stack[top] = node;

}

nodeptr pop() {
	int temp = 0;
	if (top < 0)
	{
		printf("stack is empty.\n");
		return 0;
	}

	temp = top; ///>>> ���� top �� ����Ű�� ���� ���� �ֱ��� ����Ÿ�� ����Ǿ� �ִ�. ����,
	top--;
	return stack[temp];
}

/*******************************************************/
// ���ڵ� �ϳ��� �����ϴ� �Լ��̴�.
// ��ȯ��: 0: ���Խ���,  1, 2: ���Լ��� (1: ������ ����, 2:�� �� �� �þ� ��.)

int insert_arec(ele in_rec) {	//�ϳ��� ���ڵ带 ����  key = ȸ���  

	int i, j;
	nodeptr curr, child, new_ptr, tptr = NULL;
	ele empty = { "\0",0 };
	big_node bnode;

	if (!root) {	// root�� NULL�̸� btree�� �������. �� ù ��带 ����� ���⿡ �ִ´�.
		root = (nodeptr)malloc(sizeof(node));			// nodeptr���·� nodeũ�⸸ŭ �Ҵ�޾� �����ּҴ� root�� ����   
		root->rec[0] = in_rec;					// key���� root->rec[0]�� �ִ´�. 
		root->ptr[0] = root->ptr[1] = NULL;		// p0�� p1�� NULL�� �ִ´�. 
		root->fill_cnt = 1;
		return 1;  // ù ��带 ����� �ְ� ������. 
	}

	//root is not null
	curr = root;

	//   �Ʒ� �� ���� in_rec �� ���� ���� ������带 ã�� curr�� ����Ű�� �ϴ� �κ��� �;� ��!!

	do {
		for (i = 0; i < curr->fill_cnt; i++) {
			if (strcmp(in_rec.name, curr->rec[i].name) < 0) {
				// tptr = curr->ptr[i]; //  ��ģ�� �̰� �� ����?
				break;
			}

			else if (strcmp(in_rec.name, curr->rec[i].name) == 0) { // ��ģ�� ������ �� ��� �Լ��� ����?
				return 0;
			}
		}
		// ����: ���� i�� ã�� ��ġ�� ��� ����.
		// curr�� ����. child�� ���� ���� �Ѵ�.
		// ��ģ�� tptr�� child�� ��ħ
		child = curr->ptr[i];
		if (child) {
			push(curr);
			curr = child;
		}
		else
			break;

	} while (1);


	do {

		// curr node is not full

		if (curr->fill_cnt < MAXK) {
			for (i = 0; i < curr->fill_cnt; i++)
				if (strcmp(in_rec.name, curr->rec[i].name) < 0)
					break;
			for (j = curr->fill_cnt; j > i; j--) {
				curr->ptr[j + 1] = curr->ptr[j];
				curr->rec[j] = curr->rec[j - 1];
			}

			curr->rec[i] = in_rec;
			curr->ptr[i + 1] = child; // ��ģ�� tptr�� child�� ��ħ
			curr->fill_cnt++;

			return 1; // ���Լ��� (���� 1: ��Ʈ�� �߰� ���� ������).
		}


		else {
			// curr node is full
			for (i = 0; i < MAXK; i++) {
				if (strcmp(in_rec.name, curr->rec[i].name) < 0)
					break;
			}

			bnode.ptr[0] = curr->ptr[0];
			for (j = 0; j < i; j++) {
				bnode.rec[j] = curr->rec[j];
				bnode.ptr[j + 1] = curr->ptr[j + 1];
			}

			bnode.rec[j] = in_rec;
			bnode.ptr[j + 1] = child; // ��ģ�� tptr�� child�� ��ħ
			j++;

			while (i < MAXK) {
				bnode.rec[j] = curr->rec[i];
				bnode.ptr[j + 1] = curr->ptr[i + 1]; // ��ģ�� i �� ���϶� i + 1

				j++;
				i++;
			}

			//   �Ʒ� �� ���� big node �� 3 �κ����� ������ ���ݺδ� curr ��, ��� ���ڵ�� in_rec��, 
			//   �Ĺݺδ� �� ��忡 �ְ�, child�� �� �� ��带 ����Ű�� �ϴ� �κ��� �;� ��!!

			// ���ݺ�
			for (i = 0; i < HALFK; i++) {
				curr->ptr[i] = bnode.ptr[i];
				curr->rec[i] = bnode.rec[i];
			}
			curr->ptr[i] = bnode.ptr[i];
			// curr->ptr[i + 1] = NULL; // ��ģ�� ��� ���������ʳ�?

			for (int k = HALFK; k < MAXK; k++) {
				curr->rec[k] = empty;
				curr->ptr[k + 1] = NULL;
			}
			curr->fill_cnt = HALFK;

			// ��� ���ڵ�

			in_rec = bnode.rec[i];
			i++;

			// �Ĺݺ�
			new_ptr = (nodeptr)malloc(sizeof(node));

			for (j = 0; j < HALFK; j++, i++) {

				new_ptr->rec[j] = bnode.rec[i];
				new_ptr->ptr[j] = bnode.ptr[i];
				//new_ptr->ptr[j + 1] = bnode.ptr[i + 1]; // Ʋ���� �ƴѵ� �׳� for�� ������ new_ptr->ptr[j] = bnode.ptr[i];�ѹ� ���ִ� �� ������?
				//new_ptr->fill_cnt++;


			}
			// �߰��� �κ�
			new_ptr->fill_cnt = HALFK;
			new_ptr->ptr[j] = bnode.ptr[i]; // ��ģ�� �ٷ� ������ ����
			child = new_ptr;





			if (top >= 0) {		// ������ emtpy �� �ƴ� ��� 
				curr = pop();	// curr �� �θ�� curr�� ������.
			}
			else { // ������ empty �� (�� curr �� root �����.) �� root ��带 ����� curr �� �θ�� ��.  
				tptr = (nodeptr)malloc(sizeof(node));
				tptr->rec[0] = bnode.rec[HALFK]; // ��ģ�� in_rec�� �ƴ϶� bnode.rec ���� �;���
				tptr->ptr[0] = curr;
				tptr->ptr[1] = child; // ��ģ�� new_ptr�� child�� ��ħ��
				tptr->fill_cnt = 1;
				root = tptr;
				total_height++;
				return 2; // ���� ���� (���� 2: �� ��Ʈ�� ����)
			} // else.
		} // else.

	} while (1);

	return 0; // �� ������ ������ ���� ����.
} //�Լ� insert_arec

void insert_btree() {	//������ü�� ���ڵ带 ���� ->insert_arec �� ȣ�� 
	FILE* fp;
	ele data;
	char name_i[20], line[200];
	char* ret_fgets;
	int num, r;
	double score;
	int n = 0, lineleng;
	int check, count = 0;
	fp = fopen("Com_names1.txt", "r");
	if (fp == NULL) {
		printf("Cannot open this file :  Com_names1.txt\n");
		scanf("%d", &check);
	} //if 

	root = NULL;
	while (1) {

		ret_fgets = fgets(line, 200, fp);
		if (!ret_fgets)
			break;  // ���� ��� ����.

		lineleng = strlen(line);  // line �� ������ ���ڴ� newline ������.
		if (lineleng - 1 >= 100)
			continue; // ȸ����� �ʹ� �� ����
		line[lineleng - 1] = '\0'; //  ������ newline ���� ����.

		strcpy(data.name, line);  // ������ ���ڵ� �غ�.
		data.nleng = strlen(line);

		top = -1;  // ������ ����·� �ʱ�ȭ.
		r = insert_arec(data);   // �� ���ڵ带 ��Ʈ���� �����Ѵ�.
		if (r != 0)
			count++; // ���Լ��� ī��Ʈ ����.
	} //while 

	fp = fopen("Com_names2.txt", "r");
	if (fp == NULL) {
		printf("Cannot open this file :  Com_names2.txt\n");
		scanf("%d", &check);
	} //if 

	while (1) {
		ret_fgets = fgets(line, 200, fp);
		if (!ret_fgets)
			break;  // ���� ��� ����.

		lineleng = strlen(line);  // line �� ������ ���ڴ� newline ������.
		if (lineleng - 1 >= 100)
			continue; // ȸ����� �ʹ� �� ����
		line[lineleng - 1] = '\0'; //  ������ newline ���� ����.

		strcpy(data.name, line);  // ������ ���ڵ� �غ�.
		data.nleng = strlen(line);

		top = -1;  // ������ ����·� �ʱ�ȭ.
		r = insert_arec(data);   // �� ���ڵ带 ��Ʈ���� �����Ѵ�.
		if (r != 0)
			count++; // ���Լ��� ī��Ʈ ����.

	} //while 


	printf("���� ������ ���ڵ� �� = %d \n\n", count);
	fclose(fp);
}   //  �Լ� insert_btree

nodeptr  retrieve(char* skey, int* idx_found) {	//�˻� �Լ� 
	nodeptr curr = root;
	nodeptr P;
	int i;
	do {
		for (i = 0; i < curr->fill_cnt; i++) {
			if (strcmp(skey, curr->rec[i].name) < 0)
				break;
			else if (strcmp(skey, curr->rec[i].name) == 0) {
				*idx_found = i;
				return curr;
			}
			else
				; // do next i.
		} // for i= 
		P = curr->ptr[i];
		if (P) {
			curr = P;
		}
	} while (P);

	return NULL;

}//retrieve

int seq_scan_btree(nodeptr curr) {
	int check_stack = 0;
	int i;
	int n;
	if (curr)
	{
		n = curr->fill_cnt;
		for (i = 0; i < n; i++) {

			seq_scan_btree(curr->ptr[i]);
			printf("Name : %s\n", curr->rec[i].name);

		}
		seq_scan_btree(curr->ptr[i]);
	} //if(curr)
	else if (!curr)
	{
		return 0;
	}

	return 0;
} //seq_scan_btree

// ����������� ����, �θ��� ���ڵ�, �������� ������ �޾Ƽ� ��й踦 �ϴ� �Լ�
//  wcase: curr �� ���������� ��й��̸� ��L��, curr �� ���������� ��й��̸� ��R��.
//  j :  father �ȿ��� curr�� ����Ű�� �������� �ε�����.
void redistribution(nodeptr father, nodeptr l_sibling, nodeptr r_sibling, char wcase, int j) {
	int i, k, m, n, h;
	two_Bnode twoB; //  twobnode(bnode�� 2���� ����)

	if (wcase == 'L')
		j = j - 1;
	else if (wcase == 'R')
		j = j;

	//copy l_sibling's content, intermediate key in father, r_sibling's content to twobnode;
	for (i = 0; i < l_sibling->fill_cnt; i++) {
		twoB.ptr[i] = l_sibling->ptr[i];
		twoB.rec[i] = l_sibling->rec[i];
	}
	twoB.ptr[i] = father->ptr[j];

	// ����:  j �� father ������ l_sibling �� ���� index �� ��� ����.
	twoB.rec[i] = father->rec[j]; // �θ𿡼��� �߰� Ű�� ������.
	i++;

	for (k = 0; k < r_sibling->fill_cnt; k++, i++) {
		twoB.ptr[i] = r_sibling->ptr[k];
		twoB.rec[i] = r_sibling->rec[k];
	}
	twoB.ptr[i] = r_sibling->ptr[k];


	//Split twobnode into first half, middle record, second half;
	h = i / 2;  // h is the index of middle record.

	//copy first half to left node;
	for (n = 0; n < h; n++) {
		l_sibling->ptr[n] = twoB.ptr[n];
		l_sibling->rec[n] = twoB.rec[n];
	}
	l_sibling->ptr[n] = twoB.ptr[n];
	l_sibling->fill_cnt = n;

	//copy second half to r_sibling;
	n++;
	for (m = 0; m < (i - h - 1); m++, n++) {
		r_sibling->ptr[m] = twoB.ptr[n];
		r_sibling->rec[m] = twoB.rec[n];
	}
	r_sibling->ptr[m] = twoB.ptr[n];
	r_sibling->fill_cnt = m;

	//move the middle record to father ;
	father->rec[j] = twoB.rec[h];
} // end of redistribution

int B_tree_deletion(char* out_key) {
	nodeptr  curr, r_sibling, l_sibling, father, Pt, leftptr, rightptr;
	int i, j, k, r_OK, l_OK, found = 0, finished = 0;
	curr = root;

	// Step (0): search for a record (to be deleted) whose key equals out_key.
	do {
		for (i = 0; i < curr->fill_cnt; i++)
			if (strcmp(out_key, curr->rec[i].name) < 0)
				break;
			else if (strcmp(out_key, curr->rec[i].name) == 0) {
				found = 1; break;
			}
		if (found == 1)
			break;  // ����: ���� i�� ã�� ��ġ�� ��� ����.
		else {		// curr�� ����. child�� ���� ���� �Ѵ�.
			Pt = curr->ptr[i];
			if (Pt) {
				push(curr);
				curr = Pt;
			}
			else
				break;
		}
	} while (!found);
	if (!found) {
		return 0;
	}

	// Comes here when the key is found. It is in curr's node. i has index of rec to delete.
	// Step (1):  find successor of d_rec.
	if (curr->ptr[0]) {   // curr node is not a leaf node  
		// We need to find successor of out_key ;
		Pt = curr->ptr[i];
		push(curr);
		// ���� ���� �����͸� ���󳻷� ����.
		while (Pt) {
			push(Pt);
			Pt = Pt->ptr[0];
		}

		curr->rec[i] = Pt->rec[0];
		curr = Pt;
		i = 0;
	} //end if

	// curr ��忡�� index �� i �� ���ڵ�� �� ���� �����͸� �����Ͽ��� �Ѵ�.
	finished = false;
	do {
		// Step (2):
		//Remove record of index i and a pointer to its right from curr's node; 
		for (j = i + 1; j < curr->fill_cnt; j++) {
			curr->rec[j - 1] = curr->rec[j];
			curr->ptr[j] = curr->ptr[j + 1];
		}
		curr->fill_cnt = curr->fill_cnt - 1;

		// Step (3):
		if (curr == root) {
			if (curr->fill_cnt == 0) {
				root = root->ptr[0];
				free(curr);
			}
			return 1; // deletion succeeded.
		}

		// Step (4):
		// curr is not the root. 
		if (curr->fill_cnt >= HALFK) { return 2; } // Finish deletion with success.

		// Now, curr violates minimum capacity constraint.
		// Step (5):
		father = pop(); // bring father of curr.
		// r-sibling = pointer to right sibling of curr' node; 
		// l-sibling = pointer to left sibling of curr's node;
		for (j = 0; j <= father->fill_cnt; j++)
			if (father->ptr[j] == curr) // find ptr of father which goes down to curr.
				break;
		if (j >= 1)
			l_sibling = father->ptr[j - 1];
		else
			l_sibling = NULL;
		if (j < father->fill_cnt)
			r_sibling = father->ptr[j + 1];
		else
			r_sibling = NULL;

		// ����: father �� ptr[j] �� curr �� ����
		//  r_sibling or l_sibling  �� �ϳ��� d ���� ���� ���ڵ� ������ ��й� ������!
		r_OK = 0;  l_OK = 0;
		if (r_sibling && r_sibling->fill_cnt > HALFK)
			r_OK = 1;
		else if (l_sibling && l_sibling->fill_cnt > HALFK)
			l_OK = 1;

		if (r_OK || l_OK) {
			//if (r_sibling has more than d keys) {
			if (r_OK) {
				redistribution(father, curr, r_sibling, 'R', j);
			}
			else if (l_OK) {
				redistribution(father, l_sibling, curr, 'L', j);
			}
			printf("Redistribution has been done.\n");
			return 3; // Deletion succeeded with redistribution.
		}
		else {   //  Step 6: merging (�պ��� �ʿ���)
			//  Let leftptr be a pointer to left one of curr and sibling chosen to merge ;
			//  Let rightptr point to the right one of curr and sibling chosen to merge ;
			if (r_sibling) {
				leftptr = curr; rightptr = r_sibling;
			} // r_sibling exists.  
			else {
				leftptr = l_sibling; rightptr = curr;
			} // surely l_sibling exists.

			//   �Ʒ� �� ���� leftptr, rightptr �� ������ leftptr ������ �պ��ϴ� �κ��� �;� ��!! 
			//   ����: ���� i �� �� ���� ������ father ���� �߰� ���ڵ带 ����Ű�� �� ���ƾ� ��.
			for (i = 0; i < father->fill_cnt; i++)
			{
				if (father->ptr[i] == leftptr)
				{
					break;
				}
			}

			j = leftptr->fill_cnt;
			leftptr->rec[i] = father->rec[i];

			j++;

			for (k = 0; k < rightptr->fill_cnt; k++, j++) {
				leftptr->ptr[j] = rightptr->ptr[k];
				leftptr->rec[j] = rightptr->rec[k];
			}

			leftptr->ptr[j] = rightptr->ptr[k];
			leftptr->fill_cnt += 1 + rightptr->fill_cnt;

			free(rightptr);
			printf("Merging has been done.\n");
			curr = father;
			// Note that i has index of record in father to be deleted. 
			// Deletion of this record and pointer to its right will be done at start of next iteration.
		} // else.
	} while (!finished);  // end of do-while ��.

} // B_tree_deletion
