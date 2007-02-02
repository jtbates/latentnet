#ifndef ERGMM_STRUCTS_H
#define ERGMM_STRUCTS_H 1

#define PROP_NONE (65535-1)
#define PROP_ALL (65535-2)

/* The structure to house the state of MCMC for a given iteration.
 * Includes only values that change from iteration to iteration.
 * Also includes the "sub-states" of MCMC: proposed Z and proposed coef,
 * as well as cached values for latent distances.
 * The variables *_llk_old are to store the respective variables from the
 * previous call to the likelihood. The variables *_old (no "llk") are to store
 * the original value of MH-updated variables when they are being proposed.
 */

typedef struct {
  double **Z, *coef, **Z_mu, *Z_var, *Z_pK;
  double *sender,sender_var,*receiver,receiver_var;
  unsigned int *Z_K;
  double llk, **lpedge, lpZ, lpLV, lpcoef, lpRE, lpREV;
} ERGMM_MCMC_Par;

typedef struct {
  ERGMM_MCMC_Par *state,*prop;
  double **Z_bar,*tr_by, *pK;
  unsigned int *n;
  unsigned int prop_Z, prop_RE, prop_coef, prop_LV, prop_REV, after_Gibbs;
  unsigned int *update_order;
} ERGMM_MCMC_MCMCState;

/* The structure to house the settings of MCMC: constants that, while
 * they affect the sampling, are not a part of the posterior distribution.
 */
typedef struct {
  double Z_delta, Z_tr_delta, Z_scl_delta, RE_delta, RE_shift_delta, *coef_delta;
  double *X_means;
  unsigned int sample_size, interval;
} ERGMM_MCMC_MCMCSettings;

/* The structure to house the parameters of the prior distribution. 
 * Z_mu_var is formerly known as "muSigprior" a.k.a. \omega
 * Z_var is formely known as "Sigprior"
 * Z_var_df is formerly known as "alphaprior", the df of the prior on the
 * intracluster variance */
typedef struct {
  double Z_mu_var, Z_var, Z_var_df, *coef_mean, *coef_var, clust_dirichlet, sender_var, sender_var_df, receiver_var, receiver_var_df;
} ERGMM_MCMC_Priors;

typedef struct {
  double *llk, *lpZ, *lpcoef, *lpRE, *lpLV, *lpREV;
  double *Z, *Z_rate_move, *Z_rate_move_all, *coef, *coef_rate, *Z_mu, *Z_var, *Z_pK, *sender, *sender_var, *receiver, *receiver_var;
  int *Z_K;
} ERGMM_MCMC_ROutput;


/* The structure to house the data on which the posterior is
   conditioned, and model choice.
*/
struct ERGMM_MCMC_Model_struct{
  unsigned int dir;
  int **Y;
  double ***X;
  unsigned int **observed_ties;

  double (*lp_edge)(struct ERGMM_MCMC_Model_struct*,ERGMM_MCMC_Par*,unsigned int,unsigned int);
  double lp_Yconst;
  int *iconst;
  double *dconst;
  unsigned int verts, latent, coef, clusters;
  unsigned int sociality;
} ;

typedef struct ERGMM_MCMC_Model_struct ERGMM_MCMC_Model;

#endif