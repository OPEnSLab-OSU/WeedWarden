clc; 
clear all; 
close all; 


%% 12 Inch Test
data = readtable('Merged_Data.xlsx', 'Sheet', '12_Inch_Test_1');

ndvi = (data.v - data.s)./(data.v + data.s);


figure(1); 

plot(data.a, 'color','#7e00db', 'LineWidth',2.0) % 410 nm 
hold on 
plot(data.b, 'color','#2300ff', 'LineWidth',2.0) % 435 nm 
hold on 
plot(data.c, 'color','#007bff', 'LineWidth',2.0) % 460 nm 
hold on 
plot(data.d, 'color','#00eaff', 'LineWidth',2.0) % 485 nm 
hold on 
plot(data.e, 'color','#00ff00', 'LineWidth',2.0) % 510 nm 
hold on 
plot(data.f, 'color','#70ff00', 'LineWidth',2.0) % 535 nm 
hold on 
plot(data.g, 'color','#c3ff00', 'LineWidth',2.0) % 560 nm 
hold on 
plot(data.h, 'color','#ffef00', 'LineWidth',2.0) % 585 nm 
hold on 
plot(data.r, 'color','#ff9b00', 'LineWidth',2.0) % 610 nm 
hold on 
plot(data.i, 'color','#ff0000', 'LineWidth',2.0) % 645 nm 
hold on 
plot(data.s, 'color','#ff0000', 'LineWidth',2.0) % 680 nm 
hold on 
plot(data.j, 'color','#f60000', 'LineWidth',2.0) % 705 nm 
hold on 
plot(data.t, 'color','#c80000', 'LineWidth',2.0) % 730 nm 
hold on 
plot(data.u, 'color','#7e00db', 'LineWidth',2.0) % 760 nm 
hold on 
plot(data.v, '--ko', 'LineWidth',2.0) % 810 nm NIR not on visible spectrum  maybe make dotted line
hold on 
plot(data.w, '--kx', 'LineWidth',2.0) % 860 nm NIR not on visible spectrum  maybe make dotted line
plot(data.k, '--k', 'LineWidth',2.0) % 900 nm NIR not on visible spectrum maybe make dotted line
hold on 
plot(data.l, ':k', 'LineWidth',2.0) % 940 nm NIR not on visible spectrum  maybe make dotted line
hold on 

p1 = [5 7 7 5]; 
p1y = [0 0 750 750]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [12 14 14 12]; 
p1y = [0 0 750 750]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [18 19 19 18]; 
p1y = [0 0 750 750]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

alpha(0.3); 

legend('410','435','460','485','510','535','560','585','610','645','680','705','730','760','810','860','900','940')
%title("Combined Raw Wavelength Data for 12 inch Test")

xlabel('Packet Number')
ylabel('Relative Responsivity')

set(gca,'FontSize',30)



figure(2) 

plot(data.NDVIB, 'LineWidth', 4.0) % NDVIB Plot
hold on
plot(ndvi, 'LineWidth', 4.0) % NDVI Plot 
hold on
plot(data.EVI, 'LineWidth', 4.0) % EVI Plot

p1 = [5 7 7 5]; 
p1y = [0 0 3 3]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [12 14 14 12]; 
p1y = [0 0 3 3]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [18 19 19 18]; 
p1y = [0 0 3 3]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

alpha(0.3); 

legend('ENDVI','NDVI','EVI'); 
%title('ENDVI, NDVI, and EVI Indicies Plotted for 12 Inch Test');
xlabel('Packet Number') 
ylabel('Index Value') 
set(gca,'FontSize',30)


figure(3) 

plot(data.NDVIB, 'LineWidth',4.0) 
hold on 
plot(data.Threshold, 'k', 'LineWidth', 4.0) 

p1 = [5 7 7 5]; 
p1y = [.25 .25 .6 .6]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [12 14 14 12]; 
p1y = [.25 .25 .6 .6]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [18 19 19 18]; 
p1y = [.25 .25 .6 .6]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

alpha(0.3); 

legend('ENDVI','Calibration Threshold'); 
%title('ENDVI Index and Calibration Threshold Plotted for 12 Inch Test', 'FontSize', 24);
xlabel('Packet Number', 'FontSize', 24) 
ylabel('Index Value', 'FontSize', 24) 
set(gca,'FontSize',30)


%% Do the 16 Inch Test 
data = readtable('Merged_Data.xlsx', 'Sheet', '16_Inch_Test_1');

ndvi = (data.v - data.s)./(data.v + data.s);


figure(4); 


plot(data.a, 'color','#7e00db', 'LineWidth',2.0) % 410 nm 
hold on 
plot(data.b, 'color','#2300ff', 'LineWidth',2.0) % 435 nm 
hold on 
plot(data.c, 'color','#007bff', 'LineWidth',2.0) % 460 nm 
hold on 
plot(data.d, 'color','#00eaff', 'LineWidth',2.0) % 485 nm 
hold on 
plot(data.e, 'color','#00ff00', 'LineWidth',2.0) % 510 nm 
hold on 
plot(data.f, 'color','#70ff00', 'LineWidth',2.0) % 535 nm 
hold on 
plot(data.g, 'color','#c3ff00', 'LineWidth',2.0) % 560 nm 
hold on 
plot(data.h, 'color','#ffef00', 'LineWidth',2.0) % 585 nm 
hold on 
plot(data.r, 'color','#ff9b00', 'LineWidth',2.0) % 610 nm 
hold on 
plot(data.i, 'color','#ff0000', 'LineWidth',2.0) % 645 nm 
hold on 
plot(data.s, 'color','#ff0000', 'LineWidth',2.0) % 680 nm 
hold on 
plot(data.j, 'color','#f60000', 'LineWidth',2.0) % 705 nm 
hold on 
plot(data.t, 'color','#c80000', 'LineWidth',2.0) % 730 nm 
hold on 
plot(data.u, 'color','#7e00db', 'LineWidth',2.0) % 760 nm 
hold on 
plot(data.v, '--ko', 'LineWidth',2.0) % 810 nm NIR not on visible spectrum  maybe make dotted line
hold on 
plot(data.w, '--kx', 'LineWidth',2.0) % 860 nm NIR not on visible spectrum  maybe make dotted line
plot(data.k, '--k', 'LineWidth',2.0) % 900 nm NIR not on visible spectrum maybe make dotted line
hold on 
plot(data.l, ':k', 'LineWidth',2.0) % 940 nm NIR not on visible spectrum  maybe make dotted line
hold on 

p1 = [5 8 8 5]; 
p1y = [0 0 750 750]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [13 16 16 13]; 
p1y = [0 0 750 750]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [21 24 24 21]; 
p1y = [0 0 750 750]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

alpha(0.3); 

legend('410','435','460','485','510','535','560','585','610','645','680','705','730','760','810','860','900','940')
%title("Combined Raw Wavelength Data for 16 inch Test")

xlabel('Packet Number')
ylabel('Relative Responsivity')

set(gca,'FontSize',30)


figure(5) 

plot(data.NDVIB, 'LineWidth', 4.0) % NDVIB Plot
hold on
plot(ndvi, 'LineWidth', 4.0) % NDVI Plot 
hold on
plot(data.EVI, 'LineWidth', 4.0) % EVI Plot

p1 = [5 8 8 5]; 
p1y = [0 0 3 3]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [13 16 16 13]; 
p1y = [0 0 3 3]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [21 24 24 21]; 
p1y = [0 0 3 3]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

alpha(0.3); 

legend('ENDVI','NDVI','EVI'); 
%title('ENDVI, NDVI, and EVI Indicies Plotted for 16 Inch Test');
xlabel('Packet Number') 
ylabel('Index Value') 

set(gca,'FontSize',30)

figure(6) 

plot(data.NDVIB, 'LineWidth',4.0) 
hold on 
plot(data.Threshold, 'k', 'LineWidth', 4.0) 

p1 = [5 8 8 5]; 
p1y = [.25 .25 .6 .6]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [13 16 16 13]; 
p1y = [.25 .25 .6 .6]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

p1 = [21 24 24 21]; 
p1y = [.25 .25 .6 .6]; 
patch(p1, p1y, 'green', 'EdgeColor', 'none'); 

alpha(0.3); 

legend('ENDVI','Calibration Threshold'); 
%title('ENDVI Index and Calibration Threshold Plotted for 16 Inch Test');
xlabel('Packet Number') 
ylabel('Index Value') 
set(gca,'FontSize',30)

