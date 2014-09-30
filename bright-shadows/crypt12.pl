$text = "Gahsihegvjfhemchaywwbonsemcntmcntkslejikrmcnhqokilaknkcot";
$text2 = "4|x|sfihyfhrwofgcdufgfwnmnope
tbxqerchldthistgoslhroafibm
qosemgonadjliqcimgvoyxh
poertnbvdmsadfakjhgrueite";
$text3 = "4|x|sfihyfhrwofgcdufgfwnmnopetbxqerchldthistgoslhroafibmqosemgonadjliqcimgvoyxhpoertnbvdmsadfakjhgrueite";

$text3 =~ s/(.)(.)(.)(.)(.)/\4/g;
print $text3;
