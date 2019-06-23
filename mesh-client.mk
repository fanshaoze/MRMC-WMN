##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=mesh-client
ConfigurationName      :=Debug
WorkspacePath          := "/home/fan/codelite"
ProjectPath            := "/home/fan/codelite/mesh-client"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fan
Date                   :=14/06/19
CodeLitePath           :="/home/fan/.codelite"
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="mesh-client.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/send.c$(ObjectSuffix) $(IntermediateDirectory)/util.c$(ObjectSuffix) $(IntermediateDirectory)/command.c$(ObjectSuffix) $(IntermediateDirectory)/inform_get_428.c$(ObjectSuffix) $(IntermediateDirectory)/inform_get_864.c$(ObjectSuffix) $(IntermediateDirectory)/neighbor.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/fan/codelite/mesh-client/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM "main.c"

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) "main.c"

$(IntermediateDirectory)/send.c$(ObjectSuffix): send.c $(IntermediateDirectory)/send.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/fan/codelite/mesh-client/send.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/send.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/send.c$(DependSuffix): send.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/send.c$(ObjectSuffix) -MF$(IntermediateDirectory)/send.c$(DependSuffix) -MM "send.c"

$(IntermediateDirectory)/send.c$(PreprocessSuffix): send.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/send.c$(PreprocessSuffix) "send.c"

$(IntermediateDirectory)/util.c$(ObjectSuffix): util.c $(IntermediateDirectory)/util.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/fan/codelite/mesh-client/util.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/util.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/util.c$(DependSuffix): util.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/util.c$(ObjectSuffix) -MF$(IntermediateDirectory)/util.c$(DependSuffix) -MM "util.c"

$(IntermediateDirectory)/util.c$(PreprocessSuffix): util.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/util.c$(PreprocessSuffix) "util.c"

$(IntermediateDirectory)/command.c$(ObjectSuffix): command.c $(IntermediateDirectory)/command.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/fan/codelite/mesh-client/command.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command.c$(DependSuffix): command.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command.c$(ObjectSuffix) -MF$(IntermediateDirectory)/command.c$(DependSuffix) -MM "command.c"

$(IntermediateDirectory)/command.c$(PreprocessSuffix): command.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command.c$(PreprocessSuffix) "command.c"

$(IntermediateDirectory)/inform_get_428.c$(ObjectSuffix): inform_get_428.c $(IntermediateDirectory)/inform_get_428.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/fan/codelite/mesh-client/inform_get_428.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/inform_get_428.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/inform_get_428.c$(DependSuffix): inform_get_428.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/inform_get_428.c$(ObjectSuffix) -MF$(IntermediateDirectory)/inform_get_428.c$(DependSuffix) -MM "inform_get_428.c"

$(IntermediateDirectory)/inform_get_428.c$(PreprocessSuffix): inform_get_428.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/inform_get_428.c$(PreprocessSuffix) "inform_get_428.c"

$(IntermediateDirectory)/inform_get_864.c$(ObjectSuffix): inform_get_864.c $(IntermediateDirectory)/inform_get_864.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/fan/codelite/mesh-client/inform_get_864.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/inform_get_864.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/inform_get_864.c$(DependSuffix): inform_get_864.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/inform_get_864.c$(ObjectSuffix) -MF$(IntermediateDirectory)/inform_get_864.c$(DependSuffix) -MM "inform_get_864.c"

$(IntermediateDirectory)/inform_get_864.c$(PreprocessSuffix): inform_get_864.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/inform_get_864.c$(PreprocessSuffix) "inform_get_864.c"

$(IntermediateDirectory)/neighbor.c$(ObjectSuffix): neighbor.c $(IntermediateDirectory)/neighbor.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/fan/codelite/mesh-client/neighbor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/neighbor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/neighbor.c$(DependSuffix): neighbor.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/neighbor.c$(ObjectSuffix) -MF$(IntermediateDirectory)/neighbor.c$(DependSuffix) -MM "neighbor.c"

$(IntermediateDirectory)/neighbor.c$(PreprocessSuffix): neighbor.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/neighbor.c$(PreprocessSuffix) "neighbor.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


