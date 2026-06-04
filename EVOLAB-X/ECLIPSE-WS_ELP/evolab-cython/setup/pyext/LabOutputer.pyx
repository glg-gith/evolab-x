from libcpp.string cimport string
from libcpp.vector cimport vector

'''
Outputer
'''

# Redefine 't_output' enum type (no other way to do ??)
cdef public enum t_output:
    TO_FILE = 1
    TO_STD  = 2
    TO_BOTH = TO_FILE | TO_STD


    
cdef extern from "../inc/LabOutputerLine.h" namespace "elps" :
    cdef cppclass LabOutputerLine:
        LabOutputerLine()

        t_output outputMode
        
        void SetOuputMode(t_output mode)
        void SetFilename(string filename, bint erase)                 
        string GetFilename()                 
        void SetSeparator(string sep)                   

        void WriteData()
        void ResetDataSet()
        void AddDataLine(vector[double]& dataLine)
        void InsertText(string text)
        
        void DrawProgressBar(int len, double percent, bint erase, bint only_text)


cdef class PyLabOutputerLine:
    """
    Basic implementation for output management.
                                                                                                  
    Basically this class provides the features to produce output of data into a file, the standard
    output, or both. It manipulates DATASETs (sets of data) which are simple matrixes of doubles. 
                                                                                                  
    DATASET :

        - Each set of data is expressed as a list of list of doubles                             
        - Each time the set of data is written, it is cleared (waiting to be filled again        
        with the next data). It behaves as a buffer.                                           
                                                                                                  
    We also can customize the header and footer (See C++ LabOutputer::SetHeader(), 
    LabOutputer::SetFooter()) of the output, as well as write free entries on the fly 
    (InsertText() - in this case the text is inserted immediately, unlike DATSET which 
    are buffered until asked for writing).                       
                                                                                                  
    USAGE :
    =======
        - Instantiate                                                                            
        - Set the associated file (only required if planned to write into files)                 
        - Set the separator (which will be used to separate the data of a column)                
        - Define the mode of output (SetOuputMode())                                             
        - Buffer some data lines (AddDataLine())                                                 
        - Ask for writing the current set of data at your convenience (WriteData())              
        - Or insert some text on the fly (InsertText())                                          
                                                                                                  
    B{Note :} The "statistics" and "histogram" features are not yet implemented.                     
                                                                                                  
    """
    
    cdef LabOutputerLine *outputer

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """ 

    def __cinit__(self):
       self.outputer = new LabOutputerLine()

    def __dealloc__(self):
       if self.outputer:
           del self.outputer
    
    ###
    cpdef SetOuputMode(self, t_output mode):
        """
        SetOuputMode(self, mode)
        Set the mode of output (See LabOutputer::t_output).
        @param mode: Can be : L{cyelp.TO_FILE}, L{cyelp.TO_STD} or L{cyelp.TO_BOTH}.   
        @type mode: C{int}           
        """
        self.outputer.SetOuputMode(mode)
    
    cpdef SetFilename(self, string filename, bint erase):
        """
        SetFilename(self, filename, erase)
        Set the file name associated to this outputer (not required if we work only on standard output).
        @param filename: The full path of the file.
        @type filename: C{str}                                                       
        @param erase: If set to True, the given will be erased (if it already exists).  
        @type erase: C{bool}                          
        """
        self.outputer.SetFilename(filename, erase)
    cpdef string GetFilename(self):
        """
        GetFilename(self)
        Get the full path of the associated output file.
        @return: The full path.
        @rtype: C{str}       
        """
        return self.outputer.GetFilename()
    
    cpdef SetSeparator(self, string sep):
        """
        SetSeparator(self, string sep)
        Separator being used between written values/data (';', TAB, or else...).
        @param sep: Can be one or more characters
        @type sep: C{str}                                 
        """
        self.outputer.SetSeparator(sep)
    
    ###
    cpdef WriteData(self):
        """
        WriteData(self)
        Write the current data buffer to file (Write current dataset as text using the separator - SetSeparator()).
        """
        self.outputer.WriteData()

    cpdef ResetDataSet(self):
        """
        ResetDataSet(self)
        Clear the current set of data.
        """
        self.outputer.ResetDataSet()

    cpdef AddDataLine(self, object dataLine):
        """
        AddDataLine(self, dataLine)
        Add a line to the current set of data.              
        @param dataLine: Line of data = vector of double values.
        @type dataLine: C{list}
        """
        self.outputer.AddDataLine(dataLine)

    cpdef InsertText(self, string text):
        """
        InsertText(self, text)
        Add some text at any time (the user is responsible for carriage return with this feature).
        @param text: The text to be immediately inserted (no delay).
        @type text: C{str}                               
        """
        self.outputer.InsertText(text)

    #@classmethod
    cpdef DrawProgressBar(self, int len, double percent, bint erase = True, bint only_text = False):
        """
        DrawProgressBar(self, len, percent, erase = True, only_text = False)
        Display an ASCII progress bar.                                                                
        @param len: Length (expressed in number of characters).  
        @type len: C{int}                                      
        @param percent: The current percentage.                                                        
        @type percent: C{float}                                      
        @param erase: If the progress bar overrides itself at each iteration, or not (go to next line).
        @type erase: C{bool}                                      
        @param only_text: If the progress is only displayed as text (ex. "82%").                       
        @type only_text: C{bool}                                          
        """
        self.outputer.DrawProgressBar(len, percent, erase, only_text)
        
'''
OutputerLine
'''


