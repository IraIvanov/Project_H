DEF_CMD(HLT, 0, 3, 0, { stack_dtor(stk);
                stack_dtor(stk_addr);
                printf("stack destroied\n");
                return 0;
                break;  
                }
)
DEF_CMD(PUSH, 1, 4, 1, { if( code[ip] & RAM_FLAG ){
                    int arg = 0;
                    int tmp_ip = ip + 1;
                    if (code[ip] & IMMED_FLAG) {
                    
                        arg += *(int*)(code + tmp_ip);

                        tmp_ip += sizeof(int);
                    }

                    if ( code[ip] & REG_FLAG ) {

                        arg += (int)regs[code[tmp_ip++]];

                    }
                    ip = tmp_ip;
                    val = ram[arg];
                    stack_push(stk , val);
                    break;
                    
                }

                if ( code[ip] & REG_FLAG ) {
                        ip++;
                        int arg = (int)code[ip++];
                        val = regs[arg];
                        stack_push(stk , val);

                        break;
                }

                if (code[ip] & IMMED_FLAG) {
                    ip++;
                    val = *(el_type*)(code + ip);
                    ip += sizeof(el_type);
                    stack_push(stk , val);

                    break;
                }
                stack_push(stk , val);

                break;
                }
)


DEF_CMD(ADD, 2, 3, 0, { stack_pop(stk , &var1);
                stack_pop(stk , &var2);

                stack_push(stk , var1 + var2);
                ip++;
                break;
                }
)

DEF_CMD(SUB, 3, 3, 0, { stack_pop(stk , &var1);
                stack_pop(stk , &var2);

                stack_push(stk , var2 - var1);
                ip++;
                break;
                }
)

DEF_CMD(MUL, 4, 3, 0, { stack_pop(stk , &var1);
                stack_pop(stk , &var2);
                stack_push(stk , var1*var2);
                ip++;
                break;
                }
)

DEF_CMD(DIV, 5, 3, 0, { stack_pop(stk , &var1);
                stack_pop(stk , &var2);
                if ( !is_equal( var1 , 0 ) ){
                    printf("EXECUTING ERROR , DIVISION BY ZERO\n");
                    stack_dtor(stk);
                    stack_dtor(stk_addr);
                    return -1;
                }
                stack_push(stk , var2 / var1);
                ip++;
                break;
                }               
)

DEF_CMD(OUT, 6, 3, 0,
                {
                stack_pop(stk , &var1);
                printf("%lf\n" , var1);
                ip++;
                break;
                }
)
DEF_CMD(DUMP, 7, 4, 0, { dump(stk , LOG , 0);
                printf("dumping...\n");
                //dump_cpu(code, ip);
                ip++;
                break;
                }
)
DEF_CMD(JMP, 8, 3, 1, { ip++;
                ip = *((int*)(code + ip));
                break;
                }
)
DEF_CMD(MARK, 9, 1, 0, {
                    break;
                    }
                
)
DEF_CMD(POP, 10, 3, 1,  
                {
                stack_pop(stk , &var1);

                if( code[ip] & RAM_FLAG ){
                    int arg = 0;
                    int tmp_ip = ip + 1;

                    if (code[ip] & IMMED_FLAG) {
                        
                        arg += *(int*)(code + tmp_ip);
                        tmp_ip += sizeof(int);
                    }

                    if ( code[ip] & REG_FLAG ) {
                        
                        arg += (int)regs[code[tmp_ip++]];
                        
                    }
                    ip = tmp_ip;
                    ram[arg] = var1;

                    break;
                }

                if ( code[ip] & REG_FLAG ) {
                        ip++;
                        regs[code[ip++]] = var1;
                }
                
                break;
                }
)
DEF_CMD(DUP, 11, 3, 0,
                {
                stack_pop(stk , &var1);
                stack_push(stk , var1);
                stack_push(stk , var1);
                ip++;
                break;
                }
)
DEF_CMD(CALL, 12, 4, 1,
                {
                ip++;
                tmp = ip + sizeof(int);
                memcpy(&val , &tmp , sizeof(int));
                stack_push(stk_addr , val);
                ip = *((int*)(code + ip));
                break;
                }
)
DEF_CMD(RET, 13, 3, 0,
                {
                stack_pop(stk_addr , &val);
                memcpy(&ip , &val , sizeof(int));
                break;
                }
)
DEF_CMD(JB, 14, 2, 1,
                {
                ip++;
                stack_pop(stk , &var1);
                stack_pop(stk , &var2);
                if ( (var2 - var1) < 0  && is_equal(var1 , var2) ) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
                }
)
DEF_CMD(JBE, 15, 3, 1,
                {
                ip++;
                stack_pop(stk , &var1);
                stack_pop(stk , &var2);
                if ( (var2 - var1) <= 0 || !is_equal(var1 , var2)) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break; 
                }
)
DEF_CMD(JA, 16, 2, 1,  
                {
                ip++;
                stack_pop(stk , &var1);
                stack_pop(stk , &var2);
                if ( (var2 - var1) > 0 && is_equal(var1 , var2)) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
                }
)
DEF_CMD(JAE, 17, 3, 1, 
                {
                ip++;
                stack_pop(stk , &var1);
                stack_pop(stk , &var2);
                if ( (var2 - var1) >= 0 ||!is_equal(var1 , var2) ) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
                }
)
DEF_CMD(JE, 18, 2, 1,
                {
                ip++;
                stack_pop(stk , &var1);
                stack_pop(stk , &var2);
                if ( (var2 - var1) == 0 || !is_equal(var1 , var2 )) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;
                }
)
DEF_CMD(JNE, 19, 3, 1, 
                {
                ip++;
                stack_pop(stk, &var1);
                stack_pop(stk, &var2);
                if ( (var2 - var1) != 0 && is_equal(var1, var2) ) ip = *(int*)(code + ip);
                else ip += sizeof(int);
                break;  
                }
)
DEF_CMD(IN, 20, 2, 0,
                {
                ip++;
                scanf("%lf", &var1);
                while(getchar()!='\n'){}//printf("inputed!\n");
                stack_push(stk, var1);
                break;
                }
)
DEF_CMD(GRAPH, 21, 5, 0,
                    {
                        ip++;
                        stack_pop(stk, &var1);
                        tmp = (int)var1;
                        if ( tmp < 0 ) {
                            printf("GRAPH ERROR\n");
                            break;
                        }
                        //printf("%lu\n", tmp);
                        /*if (tmp == 10) printf("\n");
                        if (tmp == 12) printf("*");
                        if (tmp == 11) printf(" ");*/
                        for(size_t i = 0; i < RAM_SIZE; i++) {
                            if ( i == (size_t)tmp ) printf("\n");
                            if ( (int)ram[i] == 10) printf("*");
                            else printf(" ");
                        }
                        printf("\n");
                        break;
                    }
)
DEF_CMD(SIN, 22, 3, 0,
                  {
                      ip++;
                      stack_pop(stk, &var1);
                      var1 = sin(var1);
                      stack_push(stk, var1);
                  }  
)

DEF_CMD(COS, 23, 3, 0,
                  {
                      ip++;
                      stack_pop(stk, &var1);
                      var1 = cos(var1);
                      stack_push(stk, var1);
                  }  
)
DEF_CMD(NUN, 0x1F, 1488, 0, 
                {
                break;
                }           
)