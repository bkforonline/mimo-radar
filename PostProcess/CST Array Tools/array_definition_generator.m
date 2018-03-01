%% Generator array definition for CST array tools
clear;
Wh=[0.0149128480660806 + 0.00103595741760463i;-0.0459516302262829 - 0.0476937150149808i;-0.0502170565395577 + 0.0391150726770208i;0.0100741691156839 + 0.0569066339589198i;0.0252570114650339 - 0.0863579725709972i;-0.0451699904587168 - 0.0550141204897994i;0.0943173846650346 + 0.0393596573469882i;-0.0786422961488835 - 0.0892024733763846i;0.0536830254080176 + 0.0150305093199688i;-0.0517765912659192 - 0.117735909235137i;-0.0825687289597823 + 0.0240762780662774i;-0.0185386832200543 + 0.0507682107884500i;0.0349639287754190 - 0.0429002149762215i;-0.00874459960499378 - 0.0397299010425956i;0.00783513468868066 + 0.00960380589120082i;-0.000884539625293083 - 0.00269052377266775i];
Wv=[0.0108058485003922 - 1.36686024856808e-07i;0.0327168390380814 - 1.17455499196642e-08i;0.0541324372339758 + 1.99806455352518e-07i;0.0584109971274366 + 5.33260710739626e-08i;0.0943003506357431 - 1.40461862824025e-07i;0.0947877371548263 - 5.49616711824006e-08i;0.0870157629541407 + 5.38083051953635e-09i;0.133594862269341 + 8.59323937443003e-08i;0.0569820965462514 + 1.36672507798636e-07i;0.0803277715700456 + 1.17448755951579e-08i;0.0853112482646826 - 1.99795506821096e-07i;0.0649998483748965 - 5.33215095423623e-08i;0.0671598208651821 + 1.40474720818222e-07i;0.0563617466156424 + 5.49672122451494e-08i;0.0167859141104761 - 5.38172140458704e-09i;0.00630671873888642 - 8.59512205965793e-08i];

d=[0, 14.5838, 20.8875, 29.1431, 42.1765, 49.8930, 71.9496, 85.1416];
d=[d,d+106.7515];
locationX=d;
locationY=d;

tsv=zeros(16*16,9);

for jj=1:16
    for ii=1:16
    tsv(ii+16*(jj-1),:)=[ii+16*(jj-1),locationX(ii)/1000,locationY(jj)/1000,0,abs(Wh(ii))*abs(Wv(jj)), -angle(Wh(ii))/pi*180-angle(Wv(jj))/pi*180,0,0,0];
    end
end

% Copy tsv to a .tsv file and import the .tsv file in CST array tools