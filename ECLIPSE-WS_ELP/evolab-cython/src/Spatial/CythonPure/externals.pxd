from cpython.ref cimport PyObject
from libcpp.string cimport string
from libcpp.vector cimport vector

'''
MISC
'''
# Redefine 't_output' enum type (no other way to do ??)
cdef public enum t_deptype:
    IND_DEP  = 1
    SITE_DEP = 2
    ENV_DEP  = 4

'''
CALLBACK IMPL
'''
ctypedef double (*Method)(void *param, void *user_data)

# 'LabCallBack2' :: Only for reference (libelp.so compatibility)
# Should never be instantiated...
cdef extern from "../../../inc/LabCallBack2.h" namespace "elps" :
    cdef cppclass LabCallBack2:
        LabCallBack2(Method method, void *user_data)
        double cy_execute(void *parameter)
        pass
        
cdef extern from "../../../inc/ALabCallBack.h" namespace "elps" :
    cdef cppclass ALabCallBack:
        ALabCallBack(Method method, void *user_data)
        double cy_execute(void *parameter)

    
# 'callback' :: The pattern/convert method to be used
cdef inline double callback(void *parameter, void *method):
    return (<object>method)(<object>parameter)



'''
LabUserData
'''
cdef extern from "../../../inc/LabUserData.h" namespace "elps" :
    cdef cppclass LabUserData:
        #LabUserData()
        pass


'''
LabSiteBase
'''

cdef extern from "../../../inc/LabSiteBase.h" namespace "elps" :
    cdef cppclass LabSiteBase:
        #LabSiteBase()
        
        void PushAttribute(string name, t_deptype dep_type, double d_value, int i_value)
        void PopAttribute(t_deptype dep_type)
#        vector<LabAttributesStack::t_attr> GetIndAttributes();
#        vector<LabAttributesStack::t_attr> GetSiteAttributes();
#        vector<LabAttributesStack::t_attr> GetEnvAttributes();
#    
        int GetAttrInt(t_deptype dep_filter, int id)
        double GetAttrDouble(t_deptype dep_filter, int id)
        
        void SetAttrInt(t_deptype dep_filter, int id, int value)
        void SetAttrDouble(t_deptype dep_filter, int id, double value)
    
        void Copy(LabSiteBase *src_site, int dep_filter)



'''
LabSiteEvent
'''
cdef extern from "../../../inc/LabSiteEvent.h" namespace "elps" :
    cdef cppclass LabSiteEvent(LabSiteBase):
        # Direct access to LabStateStack stuffs
        int GetState()
        void SetState(int state, double time)
        void PushState(string name, int next_id, int timeout_id, double timeout)
        void PopState()
#        void GotoNextState(double time);
#        void DoTimeoutState(double time);
    
        void PushEvent(string name, double initial_rate,
                void *param,
                LabCallBack2 *rate_callback,
                LabCallBack2 *realize_callback)
        void PopEvent()
        
        # Direct access to LabStateStack stuffs
        double GetEventStackRate()
        void ResetEventStackRate()
        
        void SetTimeEvent(double time)
        double GetTimeEvent()
        void UpdateEventStackRates()
        void RealizeEvent(int id, double time)            # Realize the given event
        void RealizeEvent(double p1, double time)         # Choose an event given a probability (Multinomial low), 
                                                          # then realize it

        void Copy(LabSiteEvent *src_site, int dep_filter)


        
        

