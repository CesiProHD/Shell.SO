/*-
 * main.c
 * Minishell C source
 * Shows how to use "obtain_order" input interface function.
 *
 * Copyright (c) 1993-2002-2019, Francisco Rosales <frosal@fi.upm.es>
 * Todos los derechos reservados.
 *
 * Publicado bajo Licencia de Proyecto Educativo Práctico
 * <http://laurel.datsi.fi.upm.es/~ssoo/LICENCIA/LPEP>
 *
 * Queda prohibida la difusión total o parcial por cualquier
 * medio del material entregado al alumno para la realización
 * de este proyecto o de cualquier material derivado de este,
 * incluyendo la solución particular que desarrolle el alumno.
 *
 * DO NOT MODIFY ANYTHING OVER THIS LINE
 * THIS FILE IS TO BE MODIFIED
 */

#include <stddef.h>			/* NULL */
#include <stdio.h>			/* setbuf, printf */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define MAX_PATH_LENGTH 100

extern int obtain_order();		/* See parser.y for description */

void cd_command(char *path) {
	
    if (path == NULL) {
		path = getenv("HOME");
    }
    if (chdir(path) == 0) {
        char cwd[MAX_PATH_LENGTH];
		getcwd(cwd, sizeof(cwd));
        if (cwd == NULL) {
            perror("getcwd");
			exit(1);
        } else {
			fprintf(stdout, "%s\n", cwd);
        }
    } else {
        perror("chdir");
    }
}

void umask_command(char *mask) {
	mode_t old_mask;
	if(mask == NULL){
		old_mask = umask(0);
		umask(old_mask);
		fprintf(stdout, "%o\n", old_mask);
	} else {
		long mascara;
		char *p;
		mascara = strtol(mask, &p, 8);
		if(*p != '\0'){perror("Argumento invalido");}
		else{old_mask = umask(mascara); fprintf(stdout, "%o\n", old_mask);}
	}
}

int getrlimit_resource(char *resource){
	if (strcmp(resource, "cpu") == 0) {return 1;} 
	else if (strcmp(resource, "fsize") == 0) {return 2;} 
	else if (strcmp(resource, "data") == 0) {return 3;} 
	else if (strcmp(resource, "stack") == 0) {return 4;} 
	else if (strcmp(resource, "core") == 0) {return 5;} 
	else if (strcmp(resource, "nofile") == 0) {return 6;} 
	else {perror("Recurso no valido.\n"); return 0;}
}

void print_all_limits() {
    char *resources[] = {"cpu", "fsize", "data", "stack", "core", "nofile"};
    struct rlimit rlimit;
	long unsigned i;
    for (i = 0; i < sizeof(resources)/sizeof(resources[0]); i++) {
		switch(i){
			case 0:
				if(getrlimit(RLIMIT_CPU, &rlimit) == -1){perror("getrlimit");}
				else{fprintf(stdout, "%s\t%d\n", resources[i], (int)rlimit.rlim_cur);}
				break;
			case 1:
				if(getrlimit(RLIMIT_FSIZE, &rlimit) == -1){perror("getrlimit");}
				else{fprintf(stdout, "%s\t%d\n", resources[i], (int)rlimit.rlim_cur);}
				break;
			case 2:
				if(getrlimit(RLIMIT_DATA, &rlimit) == -1){perror("getrlimit");}
				else{fprintf(stdout, "%s\t%d\n", resources[i], (int)rlimit.rlim_cur);}
				break;
			case 3:
				if(getrlimit(RLIMIT_STACK, &rlimit) == -1){perror("getrlimit");}
				else{fprintf(stdout, "%s\t%d\n", resources[i], (int)rlimit.rlim_cur);}
				break;
			case 4:
				if(getrlimit(RLIMIT_CORE, &rlimit) == -1){perror("getrlimit");}
				else{fprintf(stdout, "%s\t%d\n", resources[i], (int)rlimit.rlim_cur);}
				break;
			default:
				if(getrlimit(RLIMIT_NOFILE, &rlimit) == -1){perror("getrlimit");}
				else{fprintf(stdout, "%s\t%d\n", resources[i], (int)rlimit.rlim_cur);}
				break;
		}	
    }
}

void limit_command(char *resource, char *limit){
	int recurso;
	struct rlimit rlimit;
	if(resource != NULL){
		recurso = getrlimit_resource(resource);
		switch(recurso){
			case 1:
				if(getrlimit(RLIMIT_CPU, &rlimit) == -1){perror("getrlimit");}
				if(limit != NULL){
					rlimit.rlim_cur = atol(limit);
					if(setrlimit(RLIMIT_CPU, &rlimit) == -1){perror("setrlimit");}
				} else{
					fprintf(stdout, "%s\t%d\n", resource, (int)rlimit.rlim_cur);
				}
				break;
			case 2:
				if(getrlimit(RLIMIT_FSIZE, &rlimit) == -1){perror("getrlimit");}
				if(limit != NULL){
					rlimit.rlim_cur = atol(limit);
					if(setrlimit(RLIMIT_FSIZE, &rlimit) == -1){perror("setrlimit");}
				} else{
					fprintf(stdout, "%s\t%d\n", resource, (int)rlimit.rlim_cur);
				}
				break;
			case 3:
				if(getrlimit(RLIMIT_DATA, &rlimit) == -1){perror("getrlimit");}
				if(limit != NULL){
					rlimit.rlim_cur = atol(limit);
					if(setrlimit(RLIMIT_DATA, &rlimit) == -1){perror("setrlimit");}
				} else{
					fprintf(stdout, "%s\t%d\n", resource, (int)rlimit.rlim_cur);
				}
				break; 
			case 4:
				if(getrlimit(RLIMIT_STACK, &rlimit) == -1){perror("getrlimit");}
				if(limit != NULL){
					rlimit.rlim_cur = atol(limit);
					if(setrlimit(RLIMIT_STACK, &rlimit) == -1){perror("setrlimit");}
				} else{
					fprintf(stdout, "%s\t%d\n", resource, (int)rlimit.rlim_cur);
				}
				break; 
			case 5:
				if(getrlimit(RLIMIT_CORE, &rlimit) == -1){perror("getrlimit");}
				if(limit != NULL){
					rlimit.rlim_cur = atol(limit);
					if(setrlimit(RLIMIT_CORE, &rlimit) == -1){perror("setrlimit");}
				} else{
					fprintf(stdout, "%s\t%d\n", resource, (int)rlimit.rlim_cur);
				}
				break;
			default: 
				if(getrlimit(RLIMIT_NOFILE, &rlimit) == -1){perror("getrlimit");}
				if(limit != NULL){
					rlimit.rlim_cur = atol(limit);
					if(setrlimit(RLIMIT_NOFILE, &rlimit) == -1){perror("setrlimit");}
				} else{
					fprintf(stdout, "%s\t%d\n", resource, (int)rlimit.rlim_cur);
				}
				break;
		}
	} else {
		print_all_limits();
	}
}

void print_environment() {
	extern char **environ;
	char **env;
    for (env = environ; *env != NULL; env++) {fprintf(stdout, "%s\n", *env);}
} 

void print_environment_var(char *var) {
    char *env_value = getenv(var);
    if (env_value != NULL) {fprintf(stdout, "%s=%s\n", var, env_value);}
}

void set_environment_var(char *var, char **value, int count) {
	int i = 2;
	char *env;
	env = malloc((count+1)*sizeof(char));
	strcpy(env, var);
	strcat(env, "=");
	while(value[i] != NULL){
		strcat(env, value[i]);
		i++;
	}
    if (putenv(var) != 0) {perror("putenv"); exit(1);}
	free(env);
}


void set_command(char *var, char **value, int count) {
	if (var == NULL) {print_environment();} 
    if (var != NULL && count == 2){print_environment_var(var);} 
	else if (var != NULL && count > 2){set_environment_var(var, value, count);}
	else{perror("Llamada invalida");}
}

int esInterno(char **argv){
	if (strcmp(argv[0], "cd") == 0)return 1;
	if (strcmp(argv[0], "umask") == 0) return 1;
	if (strcmp(argv[0], "limit") == 0)return 1;
	if (strcmp(argv[0], "set") == 0) return 1;
	if (strcmp(argv[0], "exit") == 0) return 1;
	else return 0;
}

void ejecutarInterno(char **argv){
	int count = 0;
	while (argv[count] != NULL) {
		count++;
	}
	if (strcmp(argv[0], "cd") == 0) {
		if(count > 2){perror("Num de argumentos erroneo\n");}
		else{cd_command(argv[1]);}
	} else if (strcmp(argv[0], "umask") == 0) {
		if(count > 2){perror("Num de argumentos erroneo\n");}
		else{umask_command(argv[1]);}
	} else if (strcmp(argv[0], "limit") == 0) {
		if(count > 3){perror("Num de argumentos erroneo\n");}
		else{limit_command(argv[1], argv[2]);}
	} else if (strcmp(argv[0], "set") == 0) {
		set_command(argv[1], argv, count);
	} else if (strcmp(argv[0], "exit") == 0){
		exit(0);
	}
}

int redireccionFicherosSalida(const char *filev){
	int fd = 0;
	if(filev){
		if((fd = open(filev, O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1){return fd;}
		else{
			dup2(fd, 1);
			close(fd);
		}
	}
	return fd;
}

int redireccionFicherosEntrada(const char *filev){
	int fd = 0;
	if(filev){
		if((fd = open(filev, O_RDONLY)) == -1){return fd;}
		else{
			dup2(fd, 0);
			close(fd);
		}
	}
	return fd;
}

int redireccionFicherosErr(const char *filev){
	int fd = 0;
	if(filev){
		if((fd = open(filev, O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1){return fd;}
		else{
			dup2(fd, 2);
			close(fd);
		}
	}
	return fd;
}

int main(void) {
	char ***argvv = NULL;
	int argvc; /*Contiene el numero de mandatos*/
	char **argv = NULL;
	char *filev[3] = { NULL, NULL, NULL };
	int i, status, ret, bg, entrada, salida, error;
	int fd[2];
	struct sigaction sa;
	pid_t pid;
	
	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(2, &sa, NULL);
	sigaction(3, &sa, NULL);
	
	while (1) {
		fprintf(stderr, "%s", "msh> ");	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg);
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		argvc = ret - 1;		/* Line */
		if (argvc == 0) continue;	/* Empty line */
		#if 1
/*
 * LAS LINEAS QUE A CONTINUACION SE PRESENTAN SON SOLO
 * PARA DAR UNA IDEA DE COMO UTILIZAR LAS ESTRUCTURAS
 * argvv Y filev. ESTAS LINEAS DEBERAN SER ELIMINADAS.
 */      	

			if(bg){ /*Si es background*/

				if(argvc == 1){ /*Si solo hay un mandato*/

					argv = argvv[0]; /*Obtencion de mandato*/

					pid = fork(); /*Creacion de hijo*/

					if(pid == -1){perror("fork"); exit(1);} /*Error en la creacion del hijo*/

					/*Gestion del hijo*/
					else if(pid == 0){

						/*Posible redireccion*/
						if(redireccionFicherosEntrada(filev[0]) >=0 && redireccionFicherosSalida(filev[1]) >=0 && redireccionFicherosErr(filev[2]) >= 0){ /*Si hay algun tipo de redireccion*/
							
							/*Si es interno*/
							if(esInterno(argv)){ejecutarInterno(argv); exit(0);} 
							else { /*No es interno*/
								execvp(argv[0], argv);
								perror("exec");
								exit(1);
							}

						} else { /*Si hay error al intentar abrir algun fichero de redireccion*/
							perror("open");
							exit(1);
						}
					}
				} else if(argvc > 1){ /*Si hay mas de un mandato*/

					entrada = dup(0);

					for(i = 0; (argv = argvv[i]) != NULL; i++){

						if(i == (argvc-1)){ /*Ultimo hijo tiene un tratamiento*/
							
							if(esInterno(argv)){

								salida = dup(1);
								error = dup(2);

								/*Redireccion de un mandato interno y ultimo de una secuencia*/
								if(redireccionFicherosSalida(filev[1]) >= 0 && redireccionFicherosErr(filev[2]) >= 0){
									ejecutarInterno(argv);
									if(filev[1]){dup2(salida, 1);}
									if(filev[0]){dup2(entrada, 0);}
									if(filev[2]){dup2(error, 2);}

								/*Error redireccion ficheros*/	
								} else {
									perror("open");
									exit(1);
								}

								close(salida);
								close(entrada);
								close(error);
								
							}

							else { /*Si no es interno*/

								pid = fork(); /*Creacion del hijo*/

								if(pid == -1){perror("fork"); exit(1);} /*Error en creacion del hijo*/

								/*Gestion del hijo*/
								else if(pid == 0){

									/*Activar interrupciones de las señales*/
									sa.sa_handler = SIG_DFL;
									sa.sa_flags = 0;
									sigaction(2, &sa, NULL);
									sigaction(3, &sa, NULL);
								
									/*Comprobar si hay redireccion de salida*/
									if(redireccionFicherosSalida(filev[1]) >= 0){
										execvp(argv[0], argv);
										perror("exec");
										exit(1);

									} else { /*Error en redireccion*/
										perror("open");
										exit(1);
									}
								
								} else if(pid > 0){ /*Seccion del padre*/
									dup2(entrada, 0);
									close(entrada);
								}
							}
						} else { /*Resto de hijos*/

						/*Creacion del pipe*/
						if(pipe(fd) == -1){perror("pipe"); exit(1);}

							pid = fork();
							if(pid == -1){perror("fork"); exit(1);}
							else if(pid == 0){

								/*Activacion de señales*/
								sa.sa_handler = SIG_DFL;
								sa.sa_flags = 0;
								sigaction(2, &sa, NULL);
								sigaction(3, &sa, NULL);

								/*Gestion de pipe para los hijos*/
								dup2(fd[1], 1);
								close(fd[1]);
								close(fd[0]);

								if(i == 0) { /*Primer hijo*/

									/*Si tiene redireccion de entrada o si no*/
									if(redireccionFicherosEntrada(filev[0]) >= 0){

										if(esInterno(argv)){ejecutarInterno(argv); exit(0);} /*Si es interno*/
										else{ /*No es interno*/
											execvp(argv[0], argv);
											perror("fork");
											exit(1);
										}

									} else { /*Error en el open de la redireccion de entrada*/
										perror("open");
										exit(1);
									}
								} else { /*Resto de hijos salvo el primero y el ultimo*/

									if(esInterno(argv)){ejecutarInterno(argv); exit(0);} /*Si es interno*/
									else{ /*No es interno*/
										execvp(argv[0], argv);
										perror("exec");
										exit(1);
									}
								}	
							} else if(pid > 0){
								dup2(fd[0], 0);
								close(fd[0]);
								close(fd[1]);
							}
						}
					}
				}
			} else { /*No background*/

				if(argvc == 1){ /*Un solo argumento*/

					argv = argvv[0]; /*Obtencion del mandato*/

					if(esInterno(argv)){

						entrada = dup(0);
						salida = dup(1);
						error = dup(2);

						/*Redireccion de un mandato interno y ultimo de una secuencia*/
						if(redireccionFicherosEntrada(filev[0]) >= 0 && redireccionFicherosSalida(filev[1]) >= 0 && redireccionFicherosErr(filev[2]) >= 0){
							ejecutarInterno(argv);
							if(filev[1]){dup2(salida, 1);}
							if(filev[0]){dup2(entrada, 0);}
							if(filev[2]){dup2(error, 2);}

						/*Error redireccion ficheros*/	
						} else {
							perror("open");
							exit(1);
						}

						close(salida);
						close(entrada);
						close(error);
						
					}
					else{ /*No es inteno*/

						pid = fork(); /*Creacion de hijo*/

						if(pid == -1){perror("fork");} /*Comprobacion error en creacion de hijo*/

						/*Gestion del hijo*/
						else if(pid == 0){

							/*Activacion señales*/
							sa.sa_handler = SIG_DFL;
							sa.sa_flags = 0;
							sigaction(2, &sa, NULL);
							sigaction(3, &sa, NULL);

							/*Posible redireccion*/
							if(redireccionFicherosEntrada(filev[0]) >=0 && redireccionFicherosSalida(filev[1]) >=0 && redireccionFicherosErr(filev[2]) >= 0){ /*Si hay algun tipo de redireccion*/
								execvp(argv[0], argv);
								perror("exec");
								exit(1);

							} else { /*Error en la redireccion del fichero*/
								perror("open");
								exit(1);
							}

						/*Gestion del padre*/	
						} else if(pid > 0){
							waitpid(pid, &status, 0);
						}
					}
				} else if(argvc > 1) { /*Mas de un argumento*/

					entrada = dup(0);

					for(i = 0; (argv = argvv[i]) != NULL; i++){

						if(i == (argvc-1)){ /*Ultimo hijo tiene un tratamiento*/
							
							/*Si es interno no hace falta crear hijo*/
							if(esInterno(argv)){

								salida = dup(1);
								error = dup(2);

								/*Redireccion de un mandato interno y ultimo de una secuencia*/
								if(redireccionFicherosSalida(filev[1]) >= 0 && redireccionFicherosErr(filev[2]) >= 0){
									ejecutarInterno(argv);
									if(filev[1]){dup2(salida, 1);}
									if(filev[2]){dup2(error, 2);}

								/*Error redireccion ficheros*/	
								} else {
									perror("open");
									exit(1);
								}
								
								dup2(entrada, 0);
								close(salida);
								close(entrada);
								close(error);
								
							} else { /*Si no es interno*/

								pid = fork(); /*Creacion del hijo*/

								if(pid == -1){perror("fork"); exit(1);} /*Error en creacion del hijo*/

								/*Gestion del hijo*/
								else if(pid == 0){

									/*Activar interrupciones de las señales*/
									sa.sa_handler = SIG_DFL;
									sa.sa_flags = 0;
									sigaction(2, &sa, NULL);
									sigaction(3, &sa, NULL);
								
									/*Comprobar si hay redireccion de salida*/
									if(redireccionFicherosSalida(filev[1]) >= 0 && redireccionFicherosErr(filev[2]) >= 0){
										execvp(argv[0], argv);
										perror("exec");
										exit(1);

									} else { /*Error en redireccion*/
										perror("open");
										exit(1);
									}
								
								} else { /*Seccion del padre*/
									waitpid(pid, &status, 0);
									dup2(entrada, 0);
									close(entrada);
								}
							}
						} else { /*Resto de hijos*/

						/*Creacion del pipe*/
						if(pipe(fd) == -1){perror("pipe"); exit(1);}

							pid = fork();
							if(pid == -1){perror("fork"); exit(1);}
							else if(pid == 0){

								/*Activacion de señales*/
								sa.sa_handler = SIG_DFL;
								sa.sa_flags = 0;
								sigaction(2, &sa, NULL);
								sigaction(3, &sa, NULL);

								/*Gestion de pipe para los hijos*/
								dup2(fd[1], 1);
								close(fd[1]);
								close(fd[0]);

								if(i == 0) { /*Primer hijo*/

									/*Si tiene redireccion de entrada o si no*/
									if(redireccionFicherosEntrada(filev[0]) >= 0){

										if(esInterno(argv)){ejecutarInterno(argv); exit(0);} /*Si es interno*/
										else{ /*No es interno*/
											execvp(argv[0], argv);
											perror("fork");
											exit(1);
										}

									} else { /*Error en el open de la redireccion de entrada*/
										perror("open");
										exit(1);
									}
								} else { /*Resto de hijos salvo el primero y el ultimo*/

									if(esInterno(argv)){ejecutarInterno(argv); exit(0);} /*Si es interno*/
									else{ /*No es interno*/
										execvp(argv[0], argv);
										perror("exec");
										exit(1);
									}
								}	
							} else {
								dup2(fd[0], 0);
								close(fd[0]);
								close(fd[1]);
							}
						}
					}
				}
			}
/*
 * FIN DE LA PARTE A ELIMINAR
 */
		#endif
	}
exit(0);
return 0;
}