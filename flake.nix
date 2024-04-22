{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = {self, ...} @ inputs:
    inputs.utils.lib.eachDefaultSystem (system: let
      pkgs = inputs.nixpkgs.legacyPackages.${system};
    in rec {
      formatter = pkgs.alejandra;

      devShells.default = pkgs.mkShell {
        packages = with pkgs; [
          criterion
          gcovr
          valgrind
          bear
          gdb
        ];

        inputsFrom = builtins.attrValues self.packages.${system};
      };

      packages = {
        default = packages.panoramix;
        panoramix = pkgs.stdenv.mkDerivation rec {
          name = "panoramix";
          src = ./.;

          nativeBuildInputs = [pkgs.gcc12];
          enableParallelBuilding = true;

          installPhase = ''
            mkdir -p $out/bin
            install -D ${name} $out/bin/${name}
          '';
        };
      };
    });
}
