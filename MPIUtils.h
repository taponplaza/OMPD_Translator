#ifndef MPI_UTILS_H
#define MPI_UTILS_H

#include <sstream>
#include "SymbolTable.h" 

extern bool declarePragma;
extern ofstream generatedFile;

class MPIUtils {

    private:
        std::ostringstream includes, ossp, types, impi, idt, csbp, cpl, csap, cf, ossa;
        std::ostringstream prev;
        
    public:
        void write_MPI_Type_struct(SymbolInfo* symbol){
            cpl << "\n\nMPI_Datatype MPI" << symbol->getSymbolName() << "_t;\n\n";
            cpl << "void __Declare_MPI_Type_" << symbol->getSymbolName() << "() {\n";
            cpl << "    int blocklengths[" << symbol->getParamList()->size() << "];\n";
            cpl << "    MPI_Datatype old_types[" << symbol->getParamList()->size() << "];\n";
            cpl << "    MPI_Aint disp[" << symbol->getParamList()->size() << "];\n";
            cpl << "	MPI_Aint lb;\n";
            cpl << "	MPI_Aint extent;\n";
            for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
                cpl << "    blocklengths[" << i << "] = 1;\n";
            }
            for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
                cpl << "    old_types[" << i << "] = MPI_" << symbol->getParamList()->at(i)->getVariableType() << ";\n";
            }
            for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
                cpl << "    MPI_Type_get_extent(MPI_" << symbol->getParamList()->at(i)->getVariableType() << ", &lb, &extent);\n";
                if(i == 0)
                    cpl << "    disp[" << i << "] = lb;\n";
                else
                    cpl << "    disp[" << i << "] = disp[" << i-1 << "] + extent;\n";
            }
            cpl << "    MPI_Type_create_struct(" << symbol->getParamList()->size() << ", blocklengths, disp, old_types, &MPI" << symbol->getSymbolName() << "_t);\n";
            cpl << "    MPI_Type_commit(&MPI" << symbol->getSymbolName() << "_t);\n";
            cpl << "}\n\n";
            cpl << "void Declare_MPI_Types() {\n";
            cpl << "    __Declare_MPI_Type_" << symbol->getSymbolName() << "();\n";
            cpl << "	return;\n";
            cpl << "}\n";

            idt << "    Declare_MPI_Types();\n";
        }

        void write_MPI_header(){
            includes << "# include <assert.h>\n";
            includes << "# include <mpi.h>\n";
        }

        void write_MPI_init(){
            impi << "int __taskid = -1, __numprocs = -1;\n\n";
            impi << "MPI_Init(&argc, &argv);\n";
            impi << "MPI_Comm_size(MPI_COMM_WORLD,&__numprocs);\n";
            impi << "MPI_Comm_rank(MPI_COMM_WORLD,&__taskid);\n";
        }

        void write_MPI_sec(int state){
            if (state == 4){
                csbp << "if (__taskid == 0) {\n";
                cpl << "}\n";
            }
            else if (state == 2){
                csbp << "}\n";
                cpl = std::ostringstream();
            }
            else if (state == 6){
                csap << "if (__taskid == 0) {\n";
                cf = std::ostringstream();
                cf << "}\n";
                cf << "MPI_finalize();\n";
            }
            else if (state == 5){
                csap << "}\n";
                cf = std::ostringstream();
                cf << "MPI_finalize();\n";
            }
        }

        void write_MPI_Finalice(){
            cf << "MPI_finalize();\n";
        }


        void insert_MPI(std::string token, int state){
            switch(state){
                case 0:
                    includes << prev.str();
                    break;
                case 1:
                    ossp << prev.str();
                    break;
                case 2:
                    types << prev.str();
                    break;
                case 3:
                    idt << prev.str();
                    break;
                case 4:
                    csbp << prev.str();
                    break;
                case 5:
                    cpl << prev.str();
                    break;
                case 6:
                    csap << prev.str();
                    break;
                case 7:
                    cf << prev.str();
                    break;
                case 8:
                    ossa << prev.str();
                    break;
            }
            prev = std::ostringstream();
            prev << token;
        }

        void generate_MPI_all(){
            generatedFile << includes.str() << ossp.str() << types.str()<< impi.str() << idt.str() << csbp.str() << cpl.str() << csap.str() << cf.str() << ossa.str() << prev.str();
            // generatedFile << "Testing MPIUtils\n";
        }
};

#endif // MPI_UTILS_H