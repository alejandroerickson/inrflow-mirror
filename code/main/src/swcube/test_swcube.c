// test functions not for release

static void test_n_choose_2(long n)
{
	//    01 0
	//    02 1
	//    03 2
	//    04 3
	//    05 4
	//    12 5
	//    13 6
	//    14 7
	//    15 8
	//    23 9
	//    24 10
	//    25 11
	//    34 12
	//    35 13
	//    45 14
	printf("Entering test_n_choose_2 with n=%ld\n",n);
	long *a,*b,i,j,k=0;
	a=malloc(sizeof(long));
	b=malloc(sizeof(long));
	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++) {
			*a=0;
			*b=0;
			unrank_n_choose_2(a,b,k,n);
			if((*a != i) || (*b != j))
				printf("unrank_n_choose_2 not working\n, (*a,*b,k,n)=(%ld,%ld,%ld,%ld)\n",*a,*b,k,n);
			if(k!=rank_n_choose_2(i,j,n))
				printf("rank_n_choose_2 not working\n, (j,i,n,k)=(%ld,%ld,%ld,%ld)\n",j,i,n,k);
			k++;
		}
	free(a);
	free(b);
}

