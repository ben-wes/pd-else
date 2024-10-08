// Porres 2016

#include <m_pd.h>

static t_class *lastvalue_class;

typedef struct _lastvalue{
    t_object  x_obj;
    t_float   x_last;
    t_float   x_value;
}t_lastvalue;

/*void lastvalue_float(t_lastvalue *x, t_floatarg f){
    x->x_last = f;
}*/

static t_int *lastvalue_perform(t_int *w){
    t_lastvalue *x = (t_lastvalue *)(w[1]);
    int n = (t_int)(w[2]);
    t_float *input = (t_float *)(w[3]);
    t_float *out = (t_float *)(w[4]);
    t_float last = x->x_last;
    t_float value = x->x_value;
    while(n--){
        t_float in = *input++;
        if(in != last){
            *out++ = value = last;
            last = in;
        }
        else
            *out++ = value;
    }
    x->x_last = last;
    x->x_value = value;
    return(w+5);
}

static void lastvalue_dsp(t_lastvalue *x, t_signal **sp){
    dsp_add(lastvalue_perform, 4, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec);
}

static void *lastvalue_new(t_floatarg f){
    t_lastvalue *x = (t_lastvalue *)pd_new(lastvalue_class);
    x->x_last = f;
    outlet_new(&x->x_obj, &s_signal);
    floatinlet_new(&x->x_obj, &x->x_last);
    return(x);
}

void lastvalue_tilde_setup(void){
    lastvalue_class = class_new(gensym("lastvalue~"), (t_newmethod)lastvalue_new,
        0, sizeof(t_lastvalue), CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addmethod(lastvalue_class, nullfn, gensym("signal"), 0);
//    class_addfloat(lastvalue_class, lastvalue_float);
    class_addmethod(lastvalue_class, (t_method) lastvalue_dsp, gensym("dsp"), A_CANT, 0);
}
