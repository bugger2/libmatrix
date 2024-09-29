{ pkgs ? import <nixpkgs> { } }:

pkgs.clangStdenv.mkDerivation {
  name = "matrix.h";

  nativeBuildInputs = with pkgs; [
    gdb
  ];
}
