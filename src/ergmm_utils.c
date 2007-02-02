/****************************************************************************/
/*  Author: Susan Shortreed, susanms@stat.washington.edu                    */
/*  Purpose: support functions for parameter estimation for the latent      */
/*           space model proposed by  Peter D. Hoff, Adrian E. Raftery      */
/*           and Mark S. Handcock in                                        */
/*           "Latent Space Approaches to Social Network Analysis"           */
/*           All of this code is for an R function to be incorporated       */
/*           into the R ERGM package.                                       */
/****************************************************************************/
#include <math.h>
#include <R.h>
#include <Rmath.h>
#include "matrix_utils.h"
#include "ergmm_utils.h"


#define FOUND 0
#define NOTFOUND 1

// Computes distance between every pair of points and puts the result into dist.
void pairwise_dist(double **A,unsigned int n,unsigned int dim, double **dist){
  unsigned int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<i;j++)
      dist[i][j]=dist[j][i]=dvector_dist(A[i],A[j],dim);
}

// Updates distance between vertex i and all others.
void update_dist(double **A,unsigned int i, unsigned int n, unsigned int dim, double **dist){
  unsigned int j,k;
  double temp,temp2;
  for(j=0;j<n;j++){
    temp2=0;
    for(k=0;k<dim;k++){
      temp=A[i][k]-A[j][k];
      temp2+=temp*temp;
    }
    dist[i][j]=dist[j][i]=sqrt(temp2);
  }
}

double *latentpos_average(double **A, unsigned int n, unsigned int m, double *avA){
  unsigned int i,j;
  if(!avA) avA=dvector(m);
  init_dvector(avA,m,0);
  for(j=0;j<m;j++){
    for(i=0;i<n;i++)
      avA[j]+=A[i][j];
    avA[j]/=n;
  }
  return(avA);
}

void latentpos_translate(double **A, unsigned int n, unsigned int m, double *by){
  unsigned int i,j;
  for(j=0;j<m;j++)
    for(i=0;i<n;i++)
      A[i][j]+=by[j];
}

void randeff_translate(double *v, unsigned int n, double by){
  unsigned int i;
  for(i=0;i<n;i++) v[i]+=by;
}

void add_randeff(double *effect, unsigned int n, double **eta, unsigned int is_col){
  unsigned int i,j;
  if(is_col)
    for(i=0;i<n;i++){
      for(j=0;j<n;j++){
	eta[i][j]+=effect[i];
      }
    }
  else
    for(i=0;i<n;i++){
      for(j=0;j<n;j++){
	eta[i][j]+=effect[j];
      }
    }
}

unsigned int *runifperm(unsigned int n, unsigned int *a){
  unsigned int i;
  if(!a) a=(unsigned int *) ivector(n);
  
  for(i=0;i<n;i++) a[i]=i;

  for(i=0;i<n-1;i++) uiswap(a+i, a+rdunif(i,n-1));

  return(a);
}

R_INLINE void iswap(int *a, int *b){
  int tmp=*b;
  *b=*a;
  *a=tmp;
}

R_INLINE void uiswap(unsigned int *a, unsigned int *b){
  unsigned int tmp=*b;
  *b=*a;
  *a=tmp;
}

void copy_MCMC_Par(ERGMM_MCMC_Model *model, ERGMM_MCMC_Par *source, ERGMM_MCMC_Par *dest){
  if(source->Z && (source->Z != dest->Z)) copy_dmatrix(source->Z,dest->Z,model->verts,model->latent);
  if(source->coef && (source->coef != dest->coef)) copy_dvector(source->coef,dest->coef,model->coef);
  if(source->Z_mu && (source->Z_mu != dest->Z_mu)) copy_dmatrix(source->Z_mu,dest->Z_mu,model->clusters,model->latent);
  if(source->Z_var && (source->Z_var != dest->Z_var)) copy_dvector(source->Z_var,dest->Z_var,model->clusters?model->clusters:1);
  if(source->Z_pK && (source->Z_pK != dest->Z_pK)) copy_dvector(source->Z_pK,dest->Z_pK,model->clusters);
  if(source->sender && (source->sender != dest->sender)) copy_dvector(source->sender,dest->sender,model->verts);
  if(source->sender) dest->sender_var=source->sender_var;
  if(source->receiver && (source->receiver != dest->receiver)) copy_dvector(source->receiver,dest->receiver,model->verts);
  if(source->receiver) dest->receiver_var=source->receiver_var;
  if(source->Z_K && (source->Z_K != dest->Z_K)) copy_ivector((int *) source->Z_K,(int *) dest->Z_K,model->verts);

  dest->llk=source->llk;
  // The lpedge matrix is NOT copied.
  dest->lpZ=source->lpZ;
  dest->lpLV=source->lpLV;
  dest->lpcoef=source->lpcoef;
  dest->lpRE=source->lpRE;
  dest->lpREV=source->lpREV;
}