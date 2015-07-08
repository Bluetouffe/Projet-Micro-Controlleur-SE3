clear;
clc;

cd "C:\Users\Louis\Documents\Cours\SE3\Info\Microcontrolleur\Pic\sonar_Echo.X"

for i = 1:1000
    
    if ( (i) < 13 ) then
        dbfs(i) = "{"+"0b11111111"+","+"0b11111110"+"}";
    end;
    
    if ( ( (i) > 13 ) & ( (i) <= 25 ) ) then
        dbfs(i) = "{"+"0b11111111"+","+"0b11010000"+"}";
    end;
   
    if ( ( (i) > 25 ) & ( (i) <= 50 ) ) then
        dbfs(i) = "{"+"0b11111111"+","+"0b10100001"+"}";
    end;
    
    if ( ( (i) > 50 ) & ( (i) <= 100 ) ) then
        dbfs(i) = "{"+"0b11111111"+","+"0b01000011"+"}";
    end;

    if ( ( (i) > 100 ) & ( (i) <= 150 ) ) then
        dbfs(i) = "{"+"0b11111110"+","+"0b10000111"+"}";
    end;
    
    if ( ( (i) > 150 ) & ( (i) <= 200 ) ) then
        dbfs(i) = "{"+"0b11111101"+","+"0b11001011"+"}";
    end;
    
    if ((i) > 200) then
        dbfs(i) = "{"+"0b11111101"+","+"0b11001011"+"}";
    end;
end

fd = mopen('./text_mput2.txt','wt');

mfprintf(fd , "{");

for i = 1:4:1000
    for j = 0:3
        mfprintf(fd , dbfs( i + j ));
        if (i+j < 1000)
            mfprintf(fd , ",");
        end;
    end
    if (i+j < 1000)
        mfprintf(fd , "\n");
    end;
end
mfprintf(fd , "}");
mclose(fd);
