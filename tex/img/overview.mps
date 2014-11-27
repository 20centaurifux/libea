% Metapost TeX macro
% Title: E:\git\libea\tex\img\overview.dia
% Creator: Dia v0.97.2
% CreationDate: Fri Nov 14 13:24:37 2014
% For: fedra001


beginfig(1);
verbatimtex
%&latex
\documentclass{minimal}
\begin{document}
etex
% Define macro for horizontal centering.
vardef hcentered primary P =
  P shifted -(xpart center P, 0)
enddef;
% Define macro for right justification.
vardef rjust primary P =
  P shifted -(xpart (lrcorner P - llcorner P), 0)
enddef;
  % picture(19.542682,-12.743276)(53.001048,-0.643276)
  x = 1.000000cm; y = -1.000000cm;

  t = 1.000000;

% set_line_color 0.000000, 0.000000, 0.000000
% set_line_color 1.000000, 1.000000, 1.000000
% set_linewidth 0.100000
% set_line_color 0.000000, 0.000000, 0.000000
  draw (19.592682x,2.616494y)--(19.592682x,10.423916y)--(25.935007x,10.423916y)--(25.935007x,2.616494y)--cycle
    withpen pencircle scaled 0.1000x
    dashed dashpattern (on 1.000000x off 1.000000x);
% set_linewidth 0.100000
  path p;
  p = (20.343978x,3.177370y)--(20.343978x,4.577370y)--(25.233978x,4.577370y)--(25.233978x,3.177370y)--cycle;
  fill p withcolor (1.000000,1.000000,1.000000);
% set_line_color 0.000000, 0.000000, 0.000000
  draw (20.343978x,3.177370y)--(20.343978x,4.577370y)--(25.233978x,4.577370y)--(25.233978x,3.177370y)--cycle
    withpen pencircle scaled 0.1000x;
% set_line_color 0.000000, 0.000000, 0.000000
  draw hcentered btex {\usefont{OT1}{phv}{b}{n} Sequence} etex scaled 2.05t shifted (22.788978x,4.127370y);
% set_linewidth 0.100000
  path p;
  p = (20.365591x,5.207280y)--(20.365591x,6.607280y)--(25.255591x,6.607280y)--(25.255591x,5.207280y)--cycle;
  fill p withcolor (1.000000,1.000000,1.000000);
% set_line_color 0.000000, 0.000000, 0.000000
  draw (20.365591x,5.207280y)--(20.365591x,6.607280y)--(25.255591x,6.607280y)--(25.255591x,5.207280y)--cycle
    withpen pencircle scaled 0.1000x;
% set_line_color 0.000000, 0.000000, 0.000000
  draw hcentered btex {\usefont{OT1}{phv}{b}{n} Sequence} etex scaled 2.05t shifted (22.810591x,6.157280y);
% set_linewidth 0.100000
  path p;
  p = (20.346314x,7.192871y)--(20.346314x,8.592871y)--(25.236314x,8.592871y)--(25.236314x,7.192871y)--cycle;
  fill p withcolor (1.000000,1.000000,1.000000);
% set_line_color 0.000000, 0.000000, 0.000000
  draw (20.346314x,7.192871y)--(20.346314x,8.592871y)--(25.236314x,8.592871y)--(25.236314x,7.192871y)--cycle
    withpen pencircle scaled 0.1000x;
% set_line_color 0.000000, 0.000000, 0.000000
  draw hcentered btex {\usefont{OT1}{phv}{b}{n} Sequence} etex scaled 2.05t shifted (22.791314x,8.142871y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{phv}{m}{n} ...} etex scaled 1.64t shifted (22.310821x,9.749200y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{phv}{m}{n} population} etex scaled 1.64t shifted (21.250554x,11.503460y);
% set_linewidth 0.100000
  path p;
  p = (28.196048x,0.693276y)--(28.196048x,2.893276y)--(52.951048x,2.893276y)--(52.951048x,0.693276y)--cycle;
  fill p withcolor (1.000000,1.000000,1.000000);
% set_line_color 0.000000, 0.000000, 0.000000
  draw (28.196048x,0.693276y)--(28.196048x,2.893276y)--(52.951048x,2.893276y)--(52.951048x,0.693276y)--cycle
    withpen pencircle scaled 0.1000x;
% set_line_color 0.000000, 0.000000, 0.000000
  draw hcentered btex {\usefont{OT1}{cmtt}{m}{n} <<AGenomeBase>>} etex scaled 1.84t shifted (40.573548x,1.493276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw hcentered btex {\usefont{OT1}{phv}{b}{n} GenomeBase} etex scaled 2.05t shifted (40.573548x,2.443276y);
  path p;
  p = (28.196048x,2.893276y)--(28.196048x,12.693276y)--(52.951048x,12.693276y)--(52.951048x,2.893276y)--cycle;
  fill p withcolor (1.000000,1.000000,1.000000);
% set_line_color 0.000000, 0.000000, 0.000000
  draw (28.196048x,2.893276y)--(28.196048x,12.693276y)--(52.951048x,12.693276y)--(52.951048x,2.893276y)--cycle
    withpen pencircle scaled 0.1000x;
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +dispose(in sequence:Sequence): void} etex scaled 1.84t shifted (28.346048x,3.593276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +gene_size(): uint16_t} etex scaled 1.84t shifted (28.346048x,4.393276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +create(in len:uint16_t): Sequence} etex scaled 1.84t shifted (28.346048x,5.193276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +set(in sequence:Sequence,in offset:uint16_t,} etex scaled 1.84t shifted (28.346048x,5.993276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n}      in gene:Sequence::gene_type): void} etex scaled 1.84t shifted (28.346048x,6.793276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +get(in offset:uint16_t): Sequence} etex scaled 1.84t shifted (28.346048x,7.593276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +len(in sequence:Sequence): uint16_t} etex scaled 1.84t shifted (28.346048x,8.393276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +fitness(in sequence:Sequence): float} etex scaled 1.84t shifted (28.346048x,9.193276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +hash(in sequence:Sequence): size_t} etex scaled 1.84t shifted (28.346048x,9.993276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +cmp(in a:Sequence,in b:Sequence): int} etex scaled 1.84t shifted (28.346048x,10.793276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +index_of(in seq:Sequence,in gene:Sequence::gene_type): int32_t} etex scaled 1.84t shifted (28.346048x,11.593276y);
% set_line_color 0.000000, 0.000000, 0.000000
  draw btex {\usefont{OT1}{cmtt}{m}{n} +copy(): Sequence} etex scaled 1.84t shifted (28.346048x,12.393276y);
endfig;
end;
