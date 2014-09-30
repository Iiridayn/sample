/*
   5.12.c:
   Vector inner product using 4-way loop unrolling.  Needs various support code
   to run properly, not required here; assumed.
*/

void inner_ged(vec_ptr u, vec_ptr v, data_t *dest) {
    int i;
    int length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;
    
    for(i = 0; i < length; i+=4) {
        sum = sum + udata[i] * vdata[i];
        sum = sum + udata[i+1] * vdata[i+1];
        sum = sum + udata[i+2] * vdata[i+2];
        sum = sum + udata[i+3] * vdata[i+3];
    }
    for(i -= 4; i < length; i++) {
        sum = sum + udata[i] * vdata[i];
    }
    *dest = sum;
}

