read_verilog mac32.v

synth -top mac32

dfflibmap -liberty ../../test/files/iit_stdcells/lib/tsmc018/signalstorm/iit018_stdcells.lib
abc -liberty ../../test/files/iit_stdcells/lib/tsmc018/signalstorm/iit018_stdcells.lib -liberty ../../test/files/iit_stdcells_extra/fake_ties018.lib

flatten 

# for now, we'll use fake tie cells
hilomap -hicell TIEHI Y -locell TIELO Y

splitnets -ports

clean -purge
write_verilog -noexpr mac32_netlist.v
