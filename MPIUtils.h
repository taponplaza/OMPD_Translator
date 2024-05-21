#ifndef MPI_UTILS_H
#define MPI_UTILS_H

#include <sstream>
#include "SymbolTable.h" 

extern bool declarePragma;
extern ofstream generatedFile;

class MPIUtils {
public:
    static void write_MPI_Type_struct(SymbolInfo* symbol){
        std::ostringstream oss;
        oss << "\n\nMPI_Datatype MPI" << symbol->getSymbolName() << "_t;\n\n";
        oss << "void __Declare_MPI_Type_" << symbol->getSymbolName() << "() {\n";
        oss << "    int blocklengths[" << symbol->getParamList()->size() << "];\n";
        oss << "    MPI_Datatype old_types[" << symbol->getParamList()->size() << "];\n";
        oss << "    MPI_Aint disp[" << symbol->getParamList()->size() << "];\n";
        oss << "	MPI_Aint lb;\n";
        oss << "	MPI_Aint extent;\n";
        for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
            oss << "    blocklengths[" << i << "] = 1;\n";
        }
        for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
            oss << "    old_types[" << i << "] = MPI_" << symbol->getParamList()->at(i)->getVariableType() << ";\n";
        }
        for(std::vector<SymbolInfo*>::size_type i = 0; i < symbol->getParamList()->size(); i++){
            oss << "    MPI_Type_get_extent(MPI_" << symbol->getParamList()->at(i)->getVariableType() << ", &lb, &extent);\n";
            if(i == 0)
                oss << "    disp[" << i << "] = lb;\n";
            else
                oss << "    disp[" << i << "] = disp[" << i-1 << "] + extent;\n";
        }
        oss << "    MPI_Type_create_struct(" << symbol->getParamList()->size() << ", blocklengths, disp, old_types, &MPI" << symbol->getSymbolName() << "_t);\n";
        oss << "    MPI_Type_commit(&MPI" << symbol->getSymbolName() << "_t);\n";
        oss << "}\n\n";
        oss << "void Declare_MPI_Types() {\n";
        oss << "    __Declare_MPI_Type_" << symbol->getSymbolName() << "();\n";
        oss << "	return;\n";
        oss << "}\n";

        generatedFile << oss.str();
    }

    static void write_MPI_header(){
        std::ostringstream oss;
        oss << "#include <assert.h>\n";
        oss << "#include <mpi.h>\n";
        generatedFile << oss.str();

        // idt	<< "// Aqui va la declaración de tipos si no esta vacio\n";
    }

    static void write_MPI_init(){
        std::ostringstream oss;
        oss << "	int __taskid = -1, __numprocs = -1;\n\n";
        oss << "    MPI_Init(&argc, &argv);\n";
        oss << "    MPI_Comm_size(MPI_COMM_WORLD,&__numprocs);\n";
        oss << "    MPI_Comm_rank(MPI_COMM_WORLD,&__taskid);\n";
        generatedFile << oss.str();
    }
};

#endif // MPI_UTILS_H