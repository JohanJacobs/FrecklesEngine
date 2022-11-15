pipeline {
  agent any
  stages {
    stage('build') {
      steps {
		echo 'downloading...'
        git(url: 'https://github.com/JohanJacobs/FrecklesEngine', branch: 'main')
		echo 'running premake'
		premake5 gmake2
		echo 'building'
		make		
      }	  
    }
	stage('Test'){
		steps{
			echo 'No testing implemented'
		}
	}

  }
}
