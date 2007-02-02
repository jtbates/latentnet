#ifndef WISHART_H
#define WISHART_H 1

void rdirich(unsigned int n, double *epsilon);

/* I define Scale-Inverse-Chi-Square distribution Y with a scale parameter "scale"
   and degrees of freedom "df" as

   Y=scale*df/X,
   where X~Chi-sq(df).
   
   Here, E[Y]=scale*df/(df-2)

   These functions evaluate the density and generate deviates from this distribution.
*/

#define dsclinvchisq(x,df,scale,give_log) (give_log ? (dchisq((df)*((double)scale)/((double)x),df,1)+log(((double)scale)*(df)/(double)((x)*(x)))) : (dchisq((df)*((double)scale)/((double)x),df,0)*(df)*((double)scale)/(double)((x)*(x))))

#define rsclinvchisq(df,scale) ((scale)*(df)/(rchisq(df)))

#endif