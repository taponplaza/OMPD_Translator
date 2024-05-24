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
        void write_MPI_Type_struct(SymbolInfo* symbol, int state){
            std::ostringstream css;
            css << "\n\nMPI_Datatype MPI" << symbol->getSymbolName() << "_t;\n\n";
            css << "void __Declare_MPI_Type_" << symbol->getSymbolName() << "() {\n";
            css << "    int blocklengths[" << symbol->getParamList()->size() << "];\n";
            css << "    MPI_Datatype old_types[" << symbol->getParamList()->size() << "];\n";
            css << "    MPI_Aint disp[" << symbol->getParamList()->size() << "];\n";
            css << "	MPI_Aint lb;\n";
            css << "	MPI_Aint extent;\n";
            for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
                css << "    blocklengths[" << i << "] = 1;\n";
            }
            for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
                css << "    old_types[" << i << "] = MPI_" << symbol->getParamList()->at(i)->getVariableType() << ";\n";
            }
            for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
                css << "    MPI_Type_get_extent(MPI_" << symbol->getParamList()->at(i)->getVariableType() << ", &lb, &extent);\n";
                if(i == 0)
                    css << "    disp[" << i << "] = lb;\n";
                else
                    css << "    disp[" << i << "] = disp[" << i-1 << "] + extent;\n";
            }
            css << "    MPI_Type_create_struct(" << symbol->getParamList()->size() << ", blocklengths, disp, old_types, &MPI" << symbol->getSymbolName() << "_t);\n";
            css << "    MPI_Type_commit(&MPI" << symbol->getSymbolName() << "_t);\n";
            css << "}\n\n";
            css << "void Declare_MPI_Types() {\n";
            css << "    __Declare_MPI_Type_" << symbol->getSymbolName() << "();\n";
            css << "	return;\n";
            css << "}\n";

            insert_MPI(css.str(), state);

            idt = std::ostringstream();
            idt << "\nDeclare_MPI_Types();\n";
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
                csbp << "\nif (__taskid == 0) {\n";
                cpl << "}\n";
            }
            else if (state == 2){
                csbp << "}\n";
                cpl = std::ostringstream();
            }
            else if (state == 6){
                csap << "\nif (__taskid == 0) {\n";
                cf = std::ostringstream();
                cf << "}\n";
                cf << "MPI_Finalize();\n";
            }
            else if (state == 5){
                csap << "}\n\n";
                cf = std::ostringstream();
                cf << "MPI_Finalize();\n";
            }
        }

        void write_MPI_Finalice(){
            cf << "MPI_Finalize();\n";
        }


        void insert_MPI(std::string token, int state){
            // if (token=="\n"||token=="\t"||token=="\v"||token=="\f"){
                // prev << token;
                if (false){
            }
            else{
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
        }

        void generate_MPI_all(){
            generatedFile << includes.str() << ossp.str() << types.str()<< impi.str() << idt.str() << csbp.str() << cpl.str() << csap.str() << cf.str() << ossa.str() << prev.str();
            // generatedFile << "Testing MPIUtils\n";
        }
};

#endif // MPI_UTILS_H