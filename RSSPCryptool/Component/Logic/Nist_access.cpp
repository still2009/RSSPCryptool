#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Component\Evaluation\externs.h"
#include "Component\Evaluation\cephes.h"
#include "Component\Evaluation\utilities.h"


void	partitionResultFile(int numOfFiles, int numOfSequences, int option, int testNameID);
void	postProcessResults(int option);
int		cmp(const double *a, const double *b);
int		computeMetrics(char *s, int test);
void    InitTestVector();



void
	partitionResultFile(int numOfFiles, int numOfSequences, int option, int testNameID)
{ 
	int		i, k, m, j, start, end, num, numread;
	float	c;
	FILE	**fp = (FILE **)calloc(numOfFiles+1, sizeof(FILE *));
	int		*results = (int *)calloc(numOfFiles, sizeof(int *));
	char	*s[MAXFILESPERMITTEDFORPARTITION];
	char	resultsDir[200];

	for ( i=0; i<MAXFILESPERMITTEDFORPARTITION; i++ )
		s[i] = (char*)calloc(200, sizeof(char));

	sprintf(resultsDir, "experiments/%s/%s/results.txt", generatorDir[option], testNames[testNameID]);

	if ( (fp[numOfFiles] = fopen(resultsDir, "r")) == NULL ) {
		printf("%s", resultsDir);
		printf(" -- file not found. Exiting program.\n");
		exit(-1);
	}

	for ( i=0; i<numOfFiles; i++ ) {
		if ( i < 10 )
			sprintf(s[i], "experiments/%s/%s/data%1d.txt", generatorDir[option], testNames[testNameID], i+1);
		else if (i < 100)
			sprintf(s[i], "experiments/%s/%s/data%2d.txt", generatorDir[option], testNames[testNameID], i+1);
		else
			sprintf(s[i], "experiments/%s/%s/data%3d.txt", generatorDir[option], testNames[testNameID], i+1);
	}
	numread = 0;
	m = numOfFiles/20;
	if ( (numOfFiles%20) != 0 )
		m++;
	for ( i=0; i<numOfFiles; i++ ) {
		if ( (fp[i] = fopen(s[i], "w")) == NULL ) {
			printf("%s", s[i]);
			printf(" -- file not found. Exiting program.\n");
			exit(-1);
		}
		fclose(fp[i]);
	}
	for ( num=0; num<numOfSequences; num++ ) {
		for ( k=0; k<m; k++ ) { 			/* FOR EACH BATCH */

			start = k*20;		/* BOUNDARY SEGMENTS */
			end   = k*20+19;
			if ( k == (m-1) )
				end = numOfFiles-1;

			for ( i=start; i<=end; i++ ) {		/* OPEN FILE */
				if ( (fp[i] = fopen(s[i], "a")) == NULL ) {
					printf("%s", s[i]);
					printf(" -- file not found. Exiting program.\n");
					exit(-1);
				}
			}

			for ( j=start; j<=end; j++ ) {		/* POPULATE FILE */
				fscanf(fp[numOfFiles], "%f", &c);
				fprintf(fp[j], "%f\n", c);
				numread++;
			}

			for ( i=start; i<=end; i++ )		/* CLOSE FILE */
				fclose(fp[i]);
		}
	}
	fclose(fp[numOfFiles]);
	for ( i=0; i<MAXFILESPERMITTEDFORPARTITION; i++ )
		free(s[i]);

	return;
}

int
	cmp(const void *a, const void *b)
{
	if ( *(int*)a < *(int *)b )
		return -1;
	if ( *(int*)a == *(int *)b )
		return 0;
	return 1;
}

void
	postProcessResults(int option)
{
	int		i, k, randomExcursionSampleSize, generalSampleSize;
	int		passRate, case1, case2, numOfFiles = 2;
	double	p_hat;
	char	s[200];

	for ( i=1; i<=NUMOFTESTS; i++ ) {		// FOR EACH TEST
		if ( testVector[i] ) {
			// SPECIAL CASES -- HANDLING MULTIPLE FILES FOR A SINGLE TEST
			if ( ((i == TEST_CUSUM) && testVector[TEST_CUSUM] ) ||
				((i == TEST_NONPERIODIC) && testVector[TEST_NONPERIODIC] ) ||
				((i == TEST_RND_EXCURSION) && testVector[TEST_RND_EXCURSION]) ||
				((i == TEST_RND_EXCURSION_VAR) && testVector[TEST_RND_EXCURSION_VAR]) || 
				((i == TEST_SERIAL) && testVector[TEST_SERIAL]) ) {

					if ( (i == TEST_NONPERIODIC) && testVector[TEST_NONPERIODIC] )  
						numOfFiles = MAXNUMOFTEMPLATES;
					else if ( (i == TEST_RND_EXCURSION) && testVector[TEST_RND_EXCURSION] ) 
						numOfFiles = 8;
					else if ( (i == TEST_RND_EXCURSION_VAR) && testVector[TEST_RND_EXCURSION_VAR] ) 
						numOfFiles = 18;
					else
						numOfFiles = 2;
					for ( k=0; k<numOfFiles; k++ ) {
						if ( k < 10 )
							sprintf(s, "experiments/%s/%s/data%1d.txt", generatorDir[option], testNames[i], k+1);
						else if ( k < 100 )
							sprintf(s, "experiments/%s/%s/data%2d.txt", generatorDir[option], testNames[i], k+1);
						else
							sprintf(s, "experiments/%s/%s/data%3d.txt", generatorDir[option], testNames[i], k+1);
						if ( (i == TEST_RND_EXCURSION) || (i == TEST_RND_EXCURSION_VAR) ) 
							randomExcursionSampleSize = computeMetrics(s,i);
						else
							generalSampleSize = computeMetrics(s,i);
					}
			}
			else {
				sprintf(s, "experiments/%s/%s/results.txt", generatorDir[option], testNames[i]);
				generalSampleSize = computeMetrics(s,i);
			}
		}
	}

	fprintf(summary, "\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	case1 = 0;
	case2 = 0;
	if ( testVector[TEST_RND_EXCURSION] || testVector[TEST_RND_EXCURSION_VAR] ) 
		case2 = 1;
	for ( i=1; i<=NUMOFTESTS; i++ ) {
		if ( testVector[i] && (i != TEST_RND_EXCURSION) && (i != TEST_RND_EXCURSION_VAR) ) {
			case1 = 1;
			break;
		}
	}
	p_hat = 1.0 - ALPHA;
	if ( case1 ) {
		if ( generalSampleSize == 0 ) {
			fprintf(summary, "The minimum pass rate for each statistical test with the exception of the\n");
			fprintf(summary, "random excursion (variant) test is undefined.\n\n");
		}
		else {
			passRate = (p_hat - 3.0 * sqrt((p_hat*ALPHA)/generalSampleSize)) * generalSampleSize;
			fprintf(summary, "The minimum pass rate for each statistical test with the exception of the\n");
			fprintf(summary, "random excursion (variant) test is approximately = %d for a\n", generalSampleSize ? passRate : 0);
			fprintf(summary, "sample size = %d binary sequences.\n\n", generalSampleSize);
		}
	}
	if ( case2 ) {
		if ( randomExcursionSampleSize == 0 )
			fprintf(summary, "The minimum pass rate for the random excursion (variant) test is undefined.\n\n");
		else {
			passRate = (p_hat - 3.0 * sqrt((p_hat*ALPHA)/randomExcursionSampleSize)) * randomExcursionSampleSize;
			fprintf(summary, "The minimum pass rate for the random excursion (variant) test\n");
			fprintf(summary, "is approximately = %d for a sample size = %d binary sequences.\n\n", passRate, randomExcursionSampleSize);
		}
	}
	fprintf(summary, "For further guidelines construct a probability table using the MAPLE program\n");
	fprintf(summary, "provided in the addendum section of the documentation.\n");
	fprintf(summary, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
}

int
	computeMetrics(char *s, int test)
{
	int		j, pos, count, passCount, sampleSize, expCount, proportion_threshold_min, proportion_threshold_max;
	int		freqPerBin[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double	*A, *T, chi2, proportion, uniformity, p_hat, tmp;
	float	c;
	FILE	*fp;

	if ( (fp = fopen(s, "r")) == NULL ) {
		printf("%s",s);
		printf(" -- file not found. Exiting program.\n");
		exit(-1);
	}

	if ( (A = (double *)calloc(tp.numOfBitStreams, sizeof(double))) == NULL ) {
		printf("Final Analysis Report aborted due to insufficient workspace\n");
		return 0;
	}

	/* Compute Metric 1: Proportion of Passing Sequences */

	count = 0; 		
	sampleSize = tp.numOfBitStreams;

	if ( (test == TEST_RND_EXCURSION) || (test == TEST_RND_EXCURSION_VAR) ) { /* Special Case: Random Excursion Tests */
		if ( (T = (double *)calloc(tp.numOfBitStreams, sizeof(double))) == NULL ) {
			printf("Final Analysis Report aborted due to insufficient workspace\n");
			return 0;
		}
		for ( j=0; j<sampleSize; j++ ) {
			fscanf(fp, "%f", &c);
			if ( c > 0.000000 )
				T[count++] = c;
		}

		if ( (A = (double *)calloc(count, sizeof(double))) == NULL ) {
			printf("Final Analysis Report aborted due to insufficient workspace\n");
			return 0;
		}

		for ( j=0; j<count; j++ )
			A[j] = T[j];

		sampleSize = count;
		count = 0;
		for ( j=0; j<sampleSize; j++ )
			if ( A[j] < ALPHA )
				count++;
		free(T);
	}
	else {
		if ( (A = (double *)calloc(sampleSize, sizeof(double))) == NULL ) {
			printf("Final Analysis Report aborted due to insufficient workspace\n");
			return 0;
		}
		for ( j=0; j<sampleSize; j++ ) {
			fscanf(fp, "%f", &c);
			if ( c < ALPHA )
				count++;
			A[j] = c;
		}
	}
	if ( sampleSize == 0 )
		passCount = 0;
	else
		passCount = sampleSize - count;

	p_hat = 1.0 - ALPHA;
	proportion_threshold_max = (p_hat + 3.0 * sqrt((p_hat*ALPHA)/sampleSize)) * sampleSize;
	proportion_threshold_min = (p_hat - 3.0 * sqrt((p_hat*ALPHA)/sampleSize)) * sampleSize;

	/* Compute Metric 2: Histogram */

	qsort((void *)A, sampleSize, sizeof(double), cmp);
	for ( j=0; j<sampleSize; j++ ) {
		pos = (int)floor(A[j]*10);
		if ( pos == 10 )
			pos--;
		freqPerBin[pos]++;
	}
	chi2 = 0.0;
	expCount = sampleSize/10;
	if ( expCount == 0 )
		uniformity = 0.0;
	else {
		for ( j=0; j<10; j++ )
			chi2 += pow((double)freqPerBin[j]-expCount, 2)/expCount;
		uniformity = cephes_igamc(9.0/2.0, chi2/2.0);
	}

	for ( j=0; j<10; j++ )			/* DISPLAY RESULTS */
		fprintf(summary, "%3d ", freqPerBin[j]);

	if ( expCount == 0 )
		fprintf(summary, "    ----    ");
	else if ( uniformity < 0.0001 )
		fprintf(summary, " %8.6f * ", uniformity);
	else
		fprintf(summary, " %8.6f   ", uniformity);

	if ( sampleSize == 0 )
		fprintf(summary, " ------     %s\n", testNames[test]);
	//	else if ( proportion < 0.96 )
	else if ( (passCount < proportion_threshold_min) || (passCount > proportion_threshold_max))
		fprintf(summary, "%4d/%-4d *  %s\n", passCount, sampleSize, testNames[test]);
	else
		fprintf(summary, "%4d/%-4d    %s\n", passCount, sampleSize, testNames[test]);

	fclose(fp);
	free(A);

	return sampleSize;
}

void InitTestVector()
{
	AfxMessageBox(_T("开始测试！"));
	//初始化testVector数组
	for(int i=0;i<NUMOFTESTS+1;i++)
		testVector[i] = 0;
}
