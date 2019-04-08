//������
//
//  main.c
//  Train
//
//  Created by chenyufeng on 16/1/30.
//  Copyright ? 2016�� chenyufengweb. All rights reserved.
//

#include <stdio.h>

void BuildHeap(int *a,int n);
void swap(int *a,int *b);
void HeapSort(int *a,int n);
void HeapAdjust(int *a,int i,int n);

int main(int argc,const char *argv[]){

    int a[] = {3,25,9,30,2};
    HeapSort(a, 5);
    for (int i = 0; i < 5; i++) {
        printf("%d ",a[i]);
    }

    return 0;
}

//������
void BuildHeap(int *a,int n){

    for (int i = n - 1; i >= 0; i--) {
        HeapAdjust(a, i, n);
    }

}

//����������
void swap(int *a,int *b){

    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

//������
void HeapSort(int *a,int n){

    BuildHeap(a, n);
    for (int i = n - 1; i >= 0; i--) {
        //�����Ѷ������һ��Ԫ�أ���ÿ�ν�ʣ��Ԫ���е�����߷ŵ����棻
        swap(&a[0], &a[i+1]);
        //���µ�����Ϊ�󶥶ѣ�
        HeapAdjust(a, 0, i );
    }
}

//������
void HeapAdjust(int *a,int i,int n){

    int lchild = 2 * i;//���ӽڵ㣻
    int rchild = 2 * i + 1;//�Һ��ӽڵ㣻
    int max = i;

    if (i <= n) {
        if (lchild <= n && a[lchild] > a[max]) {
            max = lchild;
        }

        if (rchild <= n && a[rchild] > a[max]) {
            max = rchild;
        }

        if (i != max) {
            swap(&a[i], &a[max]);
            //�������֮����maxΪ���ڵ���������Ƕѣ�
            HeapAdjust(a, max, n);
        }
    }
} 
