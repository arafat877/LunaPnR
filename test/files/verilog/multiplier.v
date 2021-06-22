/* Generated by Yosys 0.9+4081 (git sha1 c6681508, gcc 9.3.0-17ubuntu1~20.04 -fPIC -Os) */

(* top =  1  *)
(* src = "multiplier.v:7.1-15.10" *)
module multiplier(\a_in[0] , \a_in[1] , \a_in[2] , \a_in[3] , \b_in[0] , \b_in[1] , \b_in[2] , \b_in[3] , \data_out[0] , \data_out[1] , \data_out[2] , \data_out[3] , \data_out[4] , \data_out[5] , \data_out[6] , \data_out[7] );
  wire _000_;
  wire _001_;
  wire _002_;
  wire _003_;
  wire _004_;
  wire _005_;
  wire _006_;
  wire _007_;
  wire _008_;
  wire _009_;
  wire _010_;
  wire _011_;
  wire _012_;
  wire _013_;
  wire _014_;
  wire _015_;
  wire _016_;
  wire _017_;
  wire _018_;
  wire _019_;
  wire _020_;
  wire _021_;
  wire _022_;
  wire _023_;
  wire _024_;
  wire _025_;
  wire _026_;
  wire _027_;
  wire _028_;
  wire _029_;
  wire _030_;
  wire _031_;
  wire _032_;
  wire _033_;
  wire _034_;
  wire _035_;
  wire _036_;
  wire _037_;
  wire _038_;
  wire _039_;
  wire _040_;
  wire _041_;
  wire _042_;
  wire _043_;
  wire _044_;
  wire _045_;
  wire _046_;
  wire _047_;
  wire _048_;
  wire _049_;
  wire _050_;
  wire _051_;
  wire _052_;
  wire _053_;
  wire _054_;
  (* src = "multiplier.v:8.18-8.22" *)
  input \a_in[0] ;
  (* src = "multiplier.v:8.18-8.22" *)
  input \a_in[1] ;
  (* src = "multiplier.v:8.18-8.22" *)
  input \a_in[2] ;
  (* src = "multiplier.v:8.18-8.22" *)
  input \a_in[3] ;
  (* src = "multiplier.v:9.18-9.22" *)
  input \b_in[0] ;
  (* src = "multiplier.v:9.18-9.22" *)
  input \b_in[1] ;
  (* src = "multiplier.v:9.18-9.22" *)
  input \b_in[2] ;
  (* src = "multiplier.v:9.18-9.22" *)
  input \b_in[3] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[0] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[1] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[2] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[3] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[4] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[5] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[6] ;
  (* src = "multiplier.v:10.18-10.26" *)
  output \data_out[7] ;
  NAND2X1 _055_ (
    .A(\a_in[1] ),
    .B(\b_in[1] ),
    .Y(_000_)
  );
  NAND2X1 _056_ (
    .A(\b_in[0] ),
    .B(\a_in[0] ),
    .Y(_001_)
  );
  INVX1 _057_ (
    .A(_001_),
    .Y(\data_out[0] )
  );
  NOR2X1 _058_ (
    .A(_000_),
    .B(_001_),
    .Y(_002_)
  );
  AOI22X1 _059_ (
    .A(\b_in[0] ),
    .B(\a_in[1] ),
    .C(\b_in[1] ),
    .D(\a_in[0] ),
    .Y(_003_)
  );
  NOR2X1 _060_ (
    .A(_002_),
    .B(_003_),
    .Y(\data_out[1] )
  );
  NAND2X1 _061_ (
    .A(\a_in[0] ),
    .B(\b_in[2] ),
    .Y(_004_)
  );
  NAND2X1 _062_ (
    .A(\b_in[0] ),
    .B(\a_in[2] ),
    .Y(_005_)
  );
  NAND2X1 _063_ (
    .A(\b_in[1] ),
    .B(\a_in[2] ),
    .Y(_006_)
  );
  OR2X1 _064_ (
    .A(_000_),
    .B(_005_),
    .Y(_007_)
  );
  AND2X1 _065_ (
    .A(_000_),
    .B(_005_),
    .Y(_008_)
  );
  XOR2X1 _066_ (
    .A(_000_),
    .B(_005_),
    .Y(_009_)
  );
  NAND3X1 _067_ (
    .A(\a_in[0] ),
    .B(\b_in[2] ),
    .C(_009_),
    .Y(_010_)
  );
  XNOR2X1 _068_ (
    .A(_004_),
    .B(_009_),
    .Y(_011_)
  );
  NAND2X1 _069_ (
    .A(_002_),
    .B(_011_),
    .Y(_012_)
  );
  XOR2X1 _070_ (
    .A(_002_),
    .B(_011_),
    .Y(\data_out[2] )
  );
  OAI21X1 _071_ (
    .A(_004_),
    .B(_008_),
    .C(_007_),
    .Y(_013_)
  );
  NAND2X1 _072_ (
    .A(\b_in[0] ),
    .B(\a_in[3] ),
    .Y(_014_)
  );
  AOI22X1 _073_ (
    .A(\b_in[1] ),
    .B(\a_in[2] ),
    .C(\a_in[3] ),
    .D(\b_in[0] ),
    .Y(_015_)
  );
  NAND2X1 _074_ (
    .A(\b_in[1] ),
    .B(\a_in[3] ),
    .Y(_016_)
  );
  XNOR2X1 _075_ (
    .A(_006_),
    .B(_014_),
    .Y(_017_)
  );
  NAND2X1 _076_ (
    .A(\a_in[1] ),
    .B(\b_in[2] ),
    .Y(_018_)
  );
  XNOR2X1 _077_ (
    .A(_017_),
    .B(_018_),
    .Y(_019_)
  );
  AOI21X1 _078_ (
    .A(_007_),
    .B(_010_),
    .C(_019_),
    .Y(_020_)
  );
  NAND3X1 _079_ (
    .A(_007_),
    .B(_010_),
    .C(_019_),
    .Y(_021_)
  );
  XOR2X1 _080_ (
    .A(_013_),
    .B(_019_),
    .Y(_022_)
  );
  NAND2X1 _081_ (
    .A(\a_in[0] ),
    .B(\b_in[3] ),
    .Y(_023_)
  );
  INVX1 _082_ (
    .A(_023_),
    .Y(_024_)
  );
  XNOR2X1 _083_ (
    .A(_022_),
    .B(_024_),
    .Y(_025_)
  );
  NAND3X1 _084_ (
    .A(_002_),
    .B(_011_),
    .C(_025_),
    .Y(_026_)
  );
  AOI21X1 _085_ (
    .A(_021_),
    .B(_024_),
    .C(_020_),
    .Y(_027_)
  );
  NAND2X1 _086_ (
    .A(\a_in[1] ),
    .B(\b_in[3] ),
    .Y(_028_)
  );
  OAI22X1 _087_ (
    .A(_005_),
    .B(_016_),
    .C(_018_),
    .D(_015_),
    .Y(_029_)
  );
  NAND2X1 _088_ (
    .A(\a_in[2] ),
    .B(\b_in[2] ),
    .Y(_030_)
  );
  NAND2X1 _089_ (
    .A(\b_in[2] ),
    .B(\a_in[3] ),
    .Y(_031_)
  );
  XOR2X1 _090_ (
    .A(_016_),
    .B(_030_),
    .Y(_032_)
  );
  AND2X1 _091_ (
    .A(_029_),
    .B(_032_),
    .Y(_033_)
  );
  XNOR2X1 _092_ (
    .A(_029_),
    .B(_032_),
    .Y(_034_)
  );
  NOR2X1 _093_ (
    .A(_028_),
    .B(_034_),
    .Y(_035_)
  );
  XOR2X1 _094_ (
    .A(_028_),
    .B(_034_),
    .Y(_036_)
  );
  INVX1 _095_ (
    .A(_036_),
    .Y(_037_)
  );
  OR2X1 _096_ (
    .A(_027_),
    .B(_037_),
    .Y(_038_)
  );
  XNOR2X1 _097_ (
    .A(_027_),
    .B(_037_),
    .Y(_039_)
  );
  XOR2X1 _098_ (
    .A(_026_),
    .B(_039_),
    .Y(\data_out[4] )
  );
  OAI21X1 _099_ (
    .A(_026_),
    .B(_039_),
    .C(_038_),
    .Y(_040_)
  );
  NOR2X1 _100_ (
    .A(_033_),
    .B(_035_),
    .Y(_041_)
  );
  NAND2X1 _101_ (
    .A(\a_in[2] ),
    .B(\b_in[3] ),
    .Y(_042_)
  );
  AOI21X1 _102_ (
    .A(\b_in[1] ),
    .B(\a_in[2] ),
    .C(_031_),
    .Y(_043_)
  );
  XNOR2X1 _103_ (
    .A(_042_),
    .B(_043_),
    .Y(_044_)
  );
  INVX1 _104_ (
    .A(_044_),
    .Y(_045_)
  );
  NAND2X1 _105_ (
    .A(_041_),
    .B(_045_),
    .Y(_046_)
  );
  OAI21X1 _106_ (
    .A(_033_),
    .B(_035_),
    .C(_044_),
    .Y(_047_)
  );
  NAND2X1 _107_ (
    .A(_046_),
    .B(_047_),
    .Y(_048_)
  );
  XNOR2X1 _108_ (
    .A(_040_),
    .B(_048_),
    .Y(\data_out[5] )
  );
  AOI21X1 _109_ (
    .A(_006_),
    .B(_042_),
    .C(_031_),
    .Y(_049_)
  );
  NAND2X1 _110_ (
    .A(\a_in[3] ),
    .B(\b_in[3] ),
    .Y(_050_)
  );
  NAND3X1 _111_ (
    .A(\a_in[3] ),
    .B(\b_in[3] ),
    .C(_049_),
    .Y(_051_)
  );
  XOR2X1 _112_ (
    .A(_049_),
    .B(_050_),
    .Y(_052_)
  );
  OAI21X1 _113_ (
    .A(_027_),
    .B(_037_),
    .C(_047_),
    .Y(_053_)
  );
  NAND2X1 _114_ (
    .A(_046_),
    .B(_053_),
    .Y(_054_)
  );
  XOR2X1 _115_ (
    .A(_052_),
    .B(_054_),
    .Y(\data_out[6] )
  );
  XNOR2X1 _116_ (
    .A(_012_),
    .B(_025_),
    .Y(\data_out[3] )
  );
  OAI21X1 _117_ (
    .A(_052_),
    .B(_054_),
    .C(_051_),
    .Y(\data_out[7] )
  );
endmodule